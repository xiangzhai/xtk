xtk
===

eXtremely NOT easy ~~difficult~~ to use Tool Kit.


## Build && Install

```
./autogen.sh --prefix=/usr --enable-debug=yes
make
sudo make install
```


## Testcase

```
./xtk/test-window-x11
./xtk/test-hello-x11
```


## TODO

- [x] Theme
- [ ] Theme/oxygen
- [x] Event
- [ ] Event/MultiTouch
- [x] Event/connect
- [x] Event/disconnect
- [x] Event/ButtonPress
- [x] Event/EnterNotify
- [x] Event/LeaveNotify
- [x] Window
- [x] Window/setSize
- [x] Window/Theme
- [x] Text
- [x] Text/Theme
- [x] Image
- [x] Button
- [ ] Button/setSize
- [ ] Button/ImageButton
- [ ] Menu
- [ ] Menu/XGrabPointer
- [ ] Menu/XGrabKeyboard
- [ ] Wayland
