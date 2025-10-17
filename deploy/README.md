export QtApp=/home/truongnguyen/Working/fake/tool-log; export QMAKE=/home/truongnguyen/Qt/6.9.1/gcc_64/bin/qmake; rm -rf app tool-log-x86_64.AppImage
QMAKE=/home/truongnguyen/Qt/6.9.1/gcc_64/bin/qmake ./linuxdeploy-x86_64.AppImage --appdir app -e $QtApp/build/Desktop_Qt_6_9_1-Debug/tool-log -d ./deploy.desktop -i $QtApp/icons/tool-log.png --plugin qt --output appimage 


How to deploy:
    Download 2 images: 
        linuxdeploy-plugin-qt-x86_64.AppImage: https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases 
        linuxdeploy-x86_64.AppImage: https://github.com/linuxdeploy/linuxdeploy/releases 





    Create desktop file (deploy.desktop) 
        [Desktop Entry] 
        Name=tool-log 
        Exec=tool-log 
        Icon=tool-log 
        Type=Application 
        Categories=Utility; 
        Copy the executable file which can be run directly in local 

    Build by command: 
        QMAKE=/home/truongnguyen/Qt/6.9.1/gcc_64/bin/qmake \ 
        linuxdeploy-x86_64.AppImage \ 
        --appdir AppDir \ 
        -e ./tool-log \ 
        -d ./deploy.desktop \ 
        -i ./tool-log.png \ 
        --plugin qt \ 
        --output appimage 
