*Screenplay: Scripted Remote GUI Control*

Screenplay reads a yml file and takes connection info for libvnc, playing those instructions over the connection.

```
screenplay ./install_game.yml localhost 5900
```

Instructions are provided as a yml document. For instance:

```
- step: Clover Bootloader
  image: 000 - Clover Title.png
  confidence: 0.002
  directions:
    - clickTarget:
        image: 000 - Selected Disk.png
        confidence: 200
    - enterText: "\r\r"
- step: Utilities Menu - First Pass
  image: 001 - Disk Utility (Unselected).png
  confidence: 500
  directions:
    - clickTarget:
        image: 001 - Disk Utility (Unselected).png
        confidence: 1000
```



A properly formatted set of instructions is provided in YAML as an array of `Scene`. A `Scene` has these properties:


- `step` - Required string, a name for the Scene
- `image` - Required string, path to an image relative to the yml's location
- `confidence` - Required integer - an arbitrary number plugged into TM_SQDIFF in OpenCV that determines match threshold
- `directions` - array of (`ClickRegion`, or `EnterText`) objects, optional


A `ClickRegion` is an object with these fields:
- `image` - Required string, same as for a `Scene`
- `confidence` - Required int, same threshold value as for `Scene`


A `EnterText` is a simple text field - the string value is the text entered.

The instructions are executed a `Scene` at a time,  starting with the latest one that matches the current screen state on connecting to the remote session. Once the "current" screen has been chosen, execution stays with a given scene until the next one matches. Try making your match images unique per "Scene" to keep your screenplay predictable.

**Build Requirements**
- cmake
- nasm or yasm (optional)
- gcc or clang
- bazel(isk)
- autoconf
- automake
- autogen