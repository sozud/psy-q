$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

PS-X Library Samples Directory                          October 16 1995

psx\kanji\
 |--fontdata
 |--kanjidiv
 +--kanjifnt

psx\sample\
 |--cd
 |   |--earth
 |   |--movie
 |   |--str3d
 |   +--tuto
 |--cmplr
 |   +--scratch
 |--etc
 |   |--card
 |   |   |--doc
 |   |   |--lib
 |   |   |--makecard
 |   |   +--max
 |   |--cman
 |   |--comb
 |   +--mouse
 |--graphics
 |   |--2d
 |   |--arot
 |   |--balls
 |   |--bg
 |   |   |--bgsample
 |   |   +--fix32
 |   |--clutfog
 |   |--data
 |   |--diffuse
 |   |--divide
 |   |   |--active
 |   |   +--clip
 |   |--gsgpu
 |   |--mat
 |   |--mesh
 |   |   |--qmesh
 |   |   |--rmesh
 |   |   +--smesh
 |   |--oden
 |   |--phong
 |   |--pmd
 |   |--ppm
 |   |--rgb24
 |   |--shadow
 |   |--tmdview
 |   |   |--data
 |   |   |--lowlevel
 |   |   |--mime
 |   |   |--rcube
 |   |   |--shuttle
 |   |   |--tmdview3
 |   |   |--tmdview4
 |   |   +--tmdview5
 |   |--tod
 |   |   |--janken
 |   |   |   |--tmd
 |   |   |   +--tod
 |   |   +--todview
 |   |--trr
 |   |--tuto
 |   |--zimen
 |   +--walk
 |--math
 |   +--tree
 |--module
 |   |--execmenu
 |   |   |--anim
 |   |   |--balls
 |   |   +--rcube
 |   +--overmenu
 |--press
 |   +--tuto
 |       +--msiro
 +--sound
     |--balls
     |--basic
     |--cdvol
     |--mutual
     |--simple
     |--stream
     |--tuto
     +--xse

psx\utility\
 |--cdexec
 +--screen

-------------------------------------------------------------------------------

PS-X Library Samples Index                              October 16 1995
(library order)

[kanji] Font Data

.\fontdata: Font data
        Size and type
                11/13/15 dots.
                non-kanji/first level/second level/vertical/half-size
                characters.
        Code conversion table

.\kanjidiv: Command to extract character data
        Extracting image data from font data in character units.
        Command and viewer for extracted image data.

.\kanjifnt: Use of font data by size
        Sample to use font data by size

[sample] Sample Program

.\cmplr\scratch: Scratch pad area used Sample
         Data is placed on the scratch pad area, and the difference of the 
         processing speed can be seen with 3 access methods.

.\math\tree: Math-libaray-used sample
        Drawing a tree curve by the trigonometric function. 

.\module: Shared library
        execmenu: EXEC sample
            BALLS, RCUBE, and ANIM are activated from the menu.
            There are 2 examples in this sample.
               * Activated by LoadExec().
               * Activated by Exec() after reading to the available
                 memory.
        overmenu: Overlay sample
            BALLS, RCUBE, and ANIM are activated from the menu.
            3 BIN files are executed in order with the sound on.

.\etc\card: Memory card boot sample
        makecard: Program to create an icon for the memory card
                Creating a file with a 3-hand-gesture-image icon.
        max: Memory card operation screen 
                Disclosing source codes of the OSD operation 
                screen. 
.\etc\cman: Memory card access sample
        Memory card file utility such as state-monitoring, formatting, 
        creating.
.\etc\comb: Link cable sample
         Sample for operating each ball on the 2 machines connected to 
         each other by a link cable

.\etc\mouse: Mouse control sample
        Sample to process cursor-movements and clicking with a mouse

.\graphics\arot: Rotation angle interpolation program <libgte>
        Interpolating between a rotation angle (0,0,0) and another 
        rotation angle (a,b,c) by rotating around the principal axis.
        Based on this, the interpolation for any rotaiton angles 
        could be performed around the principal axis.
.\graphics\diffuse: Diffusion <libgte> 
        16x16 balls or rectangle polygons are diffused from the 
        origin in the world coordinate system. 
        GTE performance demonstration. 

.\graphics\divide: Polygon automatic division sample <libgte>
        clip: Divide function examples to avoid texture distortion.
               In readme.txt, PCpoly function examples included.
        active: Sub-division sample with the direct mapping.
                Crack problem and z-sorting by the maximum value not 
                by the center of gravity are included.

.\graphics\mesh: Mesh
        smesh: strip mesh
        rmesh: round mesh
        qmesh: two dimensional mesh

.\graphics\phong: phong shading <libgte>

.\graphics\pmd: PMD <libgte>
        pmdview (tmdview PMD version)
        tuto0   PMD file analysis and display
        tuto1   TMD file analysis and display 
        tuto2   PMD display in the hierarchical coordinate system
        tuto3   Light-source-calculated PMD 

.\graphics\ppm: undistorted mapping
        (perfect perspective mapping) <libgte>

.\graphics\shadow: Shadow <libgte>
         The shadow dropped from a triangle is calculated in this program.
        Since the clipping is performed accurately, the shadow can be 
        dropped on the steps

.\graphics\trr: TransRot...functions sample <libgte>
         Sample of TransRot...() functions to eliminate a gap between 
        polygons

.\graphics\walk: An object walks on a polygon <libgte>
        Constraining an object on a polygon.
        On a object (object1) created by polygons, another object 
        (object2) may move around. The object1 may take any shapes.
        The object2 changes its direction according to the direction of 
        the object1's normal.

.\graphics\2d: 2D <libgpu>
        Texture mapping on a curved surface.
        Mapping a 512x256 texture pattern on a 3-dimensional curved 
        surface.
        When 'select' is pressed, the pattern will come to pieces.

.\graphics\balls: Balls <libgpu> 
        Displaying a lot of 16x16 sprites.
        The ways of using the following functions are described here.
            FntPrint KanjiFntPrint, VSyncCallback(), VSync()

.\graphics\rgb24: RGB24 <libgpu>
        Demo in the 24-bit mode.
        Examples with StoreImage(), LoadImage(), MoveImage() are here.

.\graphics\tuto: Tutorial <libgpu>
        Step-by-step tutorial 
        tuto0   Displaying sprites 
        tuto1   Drawing test with OT
        tuto2   Drawing a rotating polygon with GTE
        tuto3   Drawing a rotating cube 
        tuto4   Drawing a cube with the light source
        tuto5   Drawing multiple 3D objects
        tuto6   Testing a 1D scrolling BG
        tuto7   Drawing a cube with the depth cueing
        tuto8   Showing a cell-type BG 
        tuto9   Showing a 3D-cell-type BG
        tuto10  Example of special effect (Mosaic)

.\graphics\bg: BG drawing function sample in libgs <libgs>
        TIM\CEL\BGD files made up with Sprite Editor may be read in 
        and displayed.
        bgsample: Back ground sample
        fix32:    Back ground sample (fast)
.\graphics\gsgpu: GSGPU <libgpu,libgs>
        Sample using libgs and libgte together.
        tuto0: Uses AddPrim() in libgs.
        tuto1: Draws libgs objects with DrawTag().
.\graphics\mat: Matchang
        Sprite animation
        The Matchang animation is placed in the 3 dimension.
.\graphics\oden: Oden <libgs> 
        Moving 3 light sources interactively, changing their colors,
        performing the real-time lighting calculation.

.\graphics\tmdview: TMD view
        A group of samples with TMD and PMD data
        data:     Directory where the data used in the samples is included.
        tmdview3: The simplest PMD data display program with GsDOBJ3 
        tmdview4: The simplest TMD data display program with GsDOBJ2 
        tmdview5: TMD data display program with GsDOBJ5
                tuto0: Simplest TMD data display program with GsDOBJ5
                tuto1: Sample of split screen 
                tuto2: With modeling data some objects are displayed.
                       More than one OT are used.
                tuto3: Automatic division with GsDOBJ5 attribute
                tuto4: Multi-screen coordinate system
                tuto5: Sample rewritten with GsSortObject5J
                tuto6: Sample where the viewpoint is moved subjectively
        rcube: rotating cubes
                Variable effect samples for 3D
        shuttle: shuttle
                Hierarchical coordinate system sample with a space-
                shuttle model.
                The Animation such as opening/closing the hatch 
                is displayed by setting the hatch and a satellite in 
                the shuttle in its child coordinate. 
        mime: MIMe Interactive Animation 
                MIMe sample program with GsDOBJ5
                Controlling 4 MIMe parameters with L1,L2,R1,R2 buttons.
                The data is a simple gauraud-shaded polygon, and MIMe 
                processing is performed for the normal.
        lowlevel: Using lowlevel functions 
            Lowlevel sample with GsTMDfast...() functions

.\graphics\tod: Animation with tod 
        janken: Multiple interactive tod animation
        todview: Simple animation

.\graphics\clutfog: Fog sample with clut
        tuto0: A clut is generated frame by frame, and transferred to 
              the vram.
        tuto1: Some cluts are placed on the vram, and switched according
              to the depth of fog.
        tuto2: Cluts are switched by DR_MOVE. 
              Applicable to drawing by libgs as well.

.\graphics\zimen: Terrain
         A group of programs to display the endless plane
        tuto0: Active primitive subdivision (with dmpsx)
        tuto1: Basic viewing volume clipping
        tuto2: Meshed ground pattern without height
        tuto3: Meshed infinite ground pattern
        tuto4: Meshed ground with active subdivision
        tuto5: Terrain sample with CLUT FOG (version with libgs)

.\cd\movie: movie <libcd>
         A group of samples of moving pictures with streaming
        tuto0: simple streaming program
        tuto1: free resolutions
        tuto2: on memory streaming

.\cd\str3d: combination
         Sample of the combination of moving pictures with streaming
        and 3D model display
         Spreading loads of animation with DecDCTvlcSize()

.\cd\earth: Earth
        Example to map a moving picture on a curved surface
.\cd\tuto: CD tutorial <libcd>
        CD-ROM step-by-step tutorial 
        tuto0: The simplest CD-Player
        tuto1: The simplest CD-Player (using callbacks)        
        tuto2: The simplest CdRead() test (background)
        tuto3: Divided reading (background) 
        tuto4: Repeating the data between two points  on CD-DA tracks.
        tuto5: Repeating the data between two points  on CD-XA tracks.
        tuto6: Multi-channel ADPCM test program
        tuto7: Program loading and executing

.\press\tuto: Tutorial <libpress>
        tuto1 - tuto6: MDEC step-by-step tutorial

.\sound\balls: combination of the sound and graphics
        Example of combination of sound and graphics.
        While pressing a button, balls bounce in the screen.
        When they hit against the wall, different sounds for each 
        ball are generated.
        SEQ data is used as the background music.
.\sound\basic: Basic play 1
        SEQ/SEP data processing function sample.
        SEQ and SEP data may be played simultaneously.
        SEP data consists of 3-connected SEQ data.

.\sound\cdvol: SPU decoded data reading sample <libspu>
        Music played on the CD is read as the "SPU decoded data" from the 
        SPU in the background, and the volume is displayed with a graph 
        (with the display of the peak level).

.\sound\tuto: Basic usage of basic sound library <libspu>
        tuto1: Pitch designation/key-on/key-off
                According to the control-pad operations, a sound is 
                played with variable pitches.
        tuto2: Mute
                Performing the sound generation, mute-on, mute-off
        tuto3: SPU interrupt
                Setting a interrupt in the middle of piano sound data.
                When the piano sound starts, and the interrupt 
                occurs, a sine wave is generated.
        tuto4: Noise sound source
                Generating a sine wave and noise by changing a pitch.
        tuto5: Divided transfer of wave form data
                Alternating divided transfer and sound 
                generation after the transfer in 2 voices.
        tuto6: Reverb
                Generating a piano sound and designating 9 kinds 
                of reverbs for the sound.

.\sound\stream: SPU streaming sample program <libspu> 
        tuto1: The background is "balls".
        tuto2: The background is "movie".

        Sample using the SPU streaming library included in the basic 
        sound library.
        Performing playback and halt of 7-channel (14 voices) SPU 
        streams by operating the control pad.
        Displaying the state of SPU streaming on the screen.

.\sound\mutual: Wave form data divided transfer sample <libsnd> 
        At a timing divided wave form data is read into the main memory, 
        and transferred to the sound buffer. This process is repeated 
        until all parts are transferred. As a result, 2 pieces of music 
        may be played.

.\sound\simple: Basic play 2
        Example using the SEQ data processing functions.
        Playing SEQ endlessly.
        On the screen the current tempo, volume, status (playing, 
        pausing, etc.) are displayed.

.\sound\xse: Auto-effect
        Example using the sound utility functions.
        Such effects as pitch-bend, auto-panning, auto-volume are 
        produced to the         keyed-on sounds.
        By moving a thumb in the scroll bar, auto-panning, auto-volume, 
        and pitch-bend may be available.

[utility]

.\cdexec: Start-up utility from CD-ROM/Emulator  
        Used when activating from CD-ROM/Emulator on H2000 with patchx 
        executed.
        Alternative module of 'resetps 0'.

.\menu: Sample Program Viewer which loads execution file.

        Sample execution files are activated from the menu.
        It is necessary that the program which can be activated from this menu
        should link "none2.obj" and be written in "menu.lst". 

.\screen: Screen 
        Frame buffer viewer.
        Demo to explain the display mode and display environment 
        parameters.

-------------------------------------------------------------------------------
