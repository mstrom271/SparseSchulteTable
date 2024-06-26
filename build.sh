#!/usr/bin/bash
set -e
set -x

# Create variables from commandline params
for param in "$@"
do
    IFS='=' read -r -a param_parts <<< "$param"
    if [[ $#param_parts[@] -eq 2 ]]; then
        param_name=$param_parts[0]
        param_value=$param_parts[1]
        declare -x "$param_name=$(eval "echo $param_value")"
    fi
done

check_variable_existence() {
    if [[ -z $!1 ]]; then
        echo "Please define $1 variable"
        exit
    fi
}

check_variable_existence Qt6_DIR
check_variable_existence OS
check_variable_existence ABI
check_variable_existence BUILD_TYPE
check_variable_existence STAGE

if [ "$OS" == "Android" ]; then
    export QT_HOST_PATH=$Qt6_DIR/../gcc_64
else
    export QT_HOST_PATH=$Qt6_DIR
fi

# Read variables from src/config.h
./build_number.sh
source config.h.sh
check_variable_existence PROJECT_NAME
check_variable_existence PROJECT_VERSION

DESTINATION_DIR=./build/$OS/$ABI/$BUILD_TYPE
mkdir -p $DESTINATION_DIR

# Translations
while IFS= read -r lang
do
    lang=$(echo "$lang" | tr -d '\r')
    $QT_HOST_PATH/bin/lupdate ./src/ -ts ./translations/translation_$lang.ts #-no-obsolete
    $QT_HOST_PATH/bin/lrelease ./translations/*.ts
done < "./translations/list.txt"
mkdir -p ./rcc/rcc
mv ./translations/*.qm ./rcc/rcc

# Resources
./rcc/rcc.sh

# Config
if [[ "$STAGE" == "All" ]] || [[ "$STAGE" == "Config" ]]; then
    rm -rf $DESTINATION_DIR/* CMakePresets.json CMakeUserPresets.json
    declare -A CONAN_PRESET
    CONAN_PRESET["Debug"]="conan-debug"
    CONAN_PRESET["Release"]="conan-release"

    conan profile detect -f
    case $OS in
        "Linux")
            conan install . \
                -s build_type=$BUILD_TYPE \
                -s compiler.cppstd=20 \
                -c tools.cmake.cmaketoolchain:generator=Ninja \
                --build=missing \
                --output-folder=$DESTINATION_DIR
            source $DESTINATION_DIR/generators/conanbuild.sh
            cmake -S . \
                -B $DESTINATION_DIR \
                --preset ${CONAN_PRESET[$BUILD_TYPE]}
            source $DESTINATION_DIR/generators/deactivate_conanbuild.sh
            ;;
        "Windows")
            conan install . \
                -s build_type=$BUILD_TYPE \
                -s compiler.cppstd=20 \
                -c tools.cmake.cmaketoolchain:generator=Ninja \
                -c tools.microsoft.bash:subsystem=msys2 \
                -c tools.microsoft.bash:active=True \
                --build=missing \
                --output-folder=$DESTINATION_DIR
            source $DESTINATION_DIR/generators/conanbuild.sh
            cmake -S . \
                -B $DESTINATION_DIR \
                --preset ${CONAN_PRESET[$BUILD_TYPE]}
            source $DESTINATION_DIR/generators/deactivate_conanbuild.sh
            ;;
        "Android")
            check_variable_existence JAVA_HOME
            check_variable_existence ANDROID_SDK_ROOT
            check_variable_existence ANDROID_NDK
            check_variable_existence API_LEVEL
            conan install . \
                -s os=$OS \
                -s arch=$ABI \
                -s build_type=$BUILD_TYPE \
                -s os.api_level=$API_LEVEL \
                -s compiler=clang \
                -s compiler.version=14 \
                -s compiler.cppstd=20 \
                -c tools.cmake.cmaketoolchain:generator=Ninja \
                -c tools.android:ndk_path=$ANDROID_NDK \
                --build=missing \
                --output-folder=$DESTINATION_DIR
            source $DESTINATION_DIR/generators/conanbuild.sh
            cmake -S . \
                -B $DESTINATION_DIR \
                --preset ${CONAN_PRESET[$BUILD_TYPE]}
            source $DESTINATION_DIR/generators/deactivate_conanbuild.sh
            ;;
    esac
    mv -f $DESTINATION_DIR/compile_commands.json ./build/
fi

# Build
if [[ "$STAGE" == "All" ]] || [[ "$STAGE" == "Build" ]]; then
    cmake --build $DESTINATION_DIR
fi

# Deploy
if [[ "$STAGE" == "All" ]] || [[ "$STAGE" == "Deploy" ]]; then
    case $OS in
        "Linux")
            cp ./android/res/drawable/icon.png ./$DESTINATION_DIR/$PROJECT_NAME.png
            cd ./$DESTINATION_DIR
            export QMAKE=$Qt6_DIR/bin/qmake
            linuxdeploy-x86_64.AppImage \
                --appdir ./$PROJECT_NAME.dir \
                --executable ./$PROJECT_NAME \
                --icon-file ./$PROJECT_NAME.png \
                --create-desktop-file \
                --output appimage \
                --plugin qt
            ;;
        "Windows")
            mkdir $DESTINATION_DIR/$PROJECT_NAME/
            cp $DESTINATION_DIR/$PROJECT_NAME.exe $DESTINATION_DIR/$PROJECT_NAME/
            windeployqt $DESTINATION_DIR/$PROJECT_NAME/$PROJECT_NAME.exe
            tar -cjvf $DESTINATION_DIR/$PROJECT_NAME.tar.bz2 $DESTINATION_DIR/$PROJECT_NAME
            ls -al $DESTINATION_DIR/
            ;;
        "Android")
            set +x
                if [ ! -n "$ANDROID_KEYSTORE_FILE" ]; then
                    export ANDROID_KEYSTORE_FILE=~/.android/debug.keystore
                    export ANDROID_KEYSTORE_PASS="android"
                    export ANDROID_KEYALIAS=androiddebugkey
                    export ANDROID_KEYALIAS_PASS="android"
                fi
                check_variable_existence ANDROID_KEYSTORE_FILE
                check_variable_existence ANDROID_KEYSTORE_PASS
                check_variable_existence ANDROID_KEYALIAS
                check_variable_existence ANDROID_KEYALIAS_PASS

                # Apk sign
                apksigner sign \
                    --ks $ANDROID_KEYSTORE_FILE \
                    --ks-pass pass:$ANDROID_KEYSTORE_PASS \
                    --ks-key-alias $ANDROID_KEYALIAS \
                    --key-pass pass:$ANDROID_KEYALIAS_PASS \
                    $DESTINATION_DIR/android-build/$PROJECT_NAME.apk

                # Android App Bundle create and sign
                if [[ "$BUILD_TYPE" == "Release" ]]; then
                    androiddeployqt \
                        --input $DESTINATION_DIR/android-$PROJECT_NAME-deployment-settings.json \
                        --output $DESTINATION_DIR/android-build/ \
                        --aab \
                        --sign $ANDROID_KEYSTORE_FILE $ANDROID_KEYALIAS \
                        --storepass $ANDROID_KEYSTORE_PASS \
                        --keypass $ANDROID_KEYALIAS_PASS
                    jarsigner -verbose -sigalg SHA256withRSA -digestalg SHA-256 \
                        $DESTINATION_DIR/android-build/build/outputs/bundle/release/android-build-release.aab \
                        -keystore $ANDROID_KEYSTORE_FILE \
                        -storepass $ANDROID_KEYSTORE_PASS $ANDROID_KEYALIAS \
                        -keypass $ANDROID_KEYALIAS_PASS
                fi
            set -x

            if [ -n "$ANDROID_DEVICE" ]; then
                adb -s $ANDROID_DEVICE install $DESTINATION_DIR/android-build/$PROJECT_NAME.apk
            fi
            ;;
    esac
fi
