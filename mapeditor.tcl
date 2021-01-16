package require Tk
wm title . "Toolbox"
wm resizable . 0 0



set e 0

set toolLbl [label .labTool -text "Tool"]
set toolList [listbox .listTool -height 0 -exportselection 0]

set tileLbl [label .labTile -text "Tile"]
set tileSpin [spinbox .spinTile -from 1 -to 65535]

set layerLbl [label .labLayer -text "Layer"]
set layerList [listbox .listLayer -height 0 -exportselection 0]

set screenXLbl [label .labXscreen -text "X"]
set screenYLbl [label .labYscreen -text "Y"]
set screenXSpin [spinbox .spinXScreen -from 0 -to 65535]
set screenYSpin [spinbox .spinYScreen -from 0 -to 65535]

set selectedPixelXYlbl [label .labPxlXY -text "Selected Pixel X Y:"]
set selectedPixelXYtext [entry .textPxlXY]

set selectedTileXYlbl [label .labTleXY -text "Selected Tile X Y:"]
set selectedTileXYtext [entry .textTleXY]

set filenameLbl [label .labFile -text "Editing file:"]
set filenameText [entry .textFile]

set scriptLbl [label .labScript -text "Map Script"]
set scriptText [text .textScript ]

.textPxlXY insert end "0 0"
.textTleXY insert end "0 0"
.textFile insert end "world.map"
.listTool insert end Stamp Fill Erase Flood-Erase Select
.listLayer insert end Bottom Collision Top All

grid $toolLbl $toolList $selectedPixelXYlbl $selectedPixelXYtext -padx 4 -pady 4 -sticky wn
grid $tileLbl $tileSpin $selectedTileXYlbl $selectedTileXYtext -padx 4 -pady 4 -sticky wn
grid $tileLbl $tileSpin -padx 4 -pady 4 -sticky wn
grid $layerLbl $layerList $filenameLbl $filenameText -padx 4 -pady 4 -sticky wn
grid $screenXLbl $screenXSpin $screenYLbl $screenYSpin -padx 4 -pady 4 -sticky wn
grid $scriptLbl -padx 4 -pady 4
grid $scriptText -padx 4 -pady 4 -sticky ew -columnspan 4
grid [button .brevert -text "Revert" -command "reloadDb"] [button .bsave -text "Save" -command "writeDb"] -padx 4 -pady 4 -sticky wn

proc sdlLoopAsync {} {
	puts [.listLayer curselection]
	sdlLoop
	after 33 [list sdlLoopAsync]
}

sdlLoopAsync