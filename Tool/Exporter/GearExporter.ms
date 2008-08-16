macroscript XportTools category:"DirectX Export Tools"
(
	
	rollout XportTools "DirectX Exporter Tools v1.0" width:264 height:235
	(
		checkbox expLevelInfo "Generate Level Data" pos:[8,8]
		GroupBox grp2 "Export Action to perform" pos:[ 8,35] width:248 height:79
		radiobuttons rdo1 "" pos:[16,56] width:147 height:48 labels:#("Export All Objects", "Export Selected Objects", "Export Selected Animation") columns:1
		edittext edt1 "Animation Range: from" pos:[12,107] width:152 height:18 enabled:false
	
		edittext edt2 "to" pos:[172,107] width:56 height:18 enabled:false
		checkbox chk1 "Show Exporter Dialog for first Object/Frame" pos:[8,142] width:243 height:30 checked:true
		button btn1 "Export" pos:[25,184] width:209 height:23


		on XportTools open do
		(
		  s = animationRange.start as string
		  e = animationRange.end as string
		  edt1.text = substring s 1 (s.count-1)
		  edt2.text = substring e 1 (e.count-1)
		)

		on rdo1 changed stat do
		(
		   if rdo1.state == 3 then
		   (
		     edt1.enabled = true
		     edt2.enabled = true
		   )
		   else
		   (
		     edt1.enabled = false
		     edt2.enabled = false
		   )
		)
		
		
		on btn1 pressed do
		(
	
	   xt = 0
	   
	   if rdo1.state == 3 then
	   (
	    f_name = getSaveFileName caption:"Save Animation as..." types:"Panda DirectX-File|*.x"
	    if f_name == undefined then xt = 1
	   )
	   else
	   (
	   	f_path = getSavePath "Choose Folder to save to"
		 if f_path == undefined then xt = 1
	   )
	   
	  if xt == 0 then
	  (
		if rdo1.state == 3 then
		(
			destroyDialog XportTools

			fnx = substring f_name 1 (f_name.count-2)
			tstr = "000000"
			frm = 0
			for i = edt1.text as integer to edt2.text as integer do
			(
			    SliderTime = i
				frm = frm + 1
				frmstr = frm as string
				if frmstr.count < 4 then
				fsub = substring tstr 1 (4-frmstr.count)
				else
				fsub = ""
				
				fnlx = fnx + fsub + frmstr + ".x"
				
				if (i == 1 and chk1.checked == true) then
			(
			
				 exportFile fnlx selectedOnly:true
	
			)
			else
			(
				exportFile fnlx #noPrompt selectedOnly:true
			)
			
			)
		
		
		)
		else
		(
			if ( expLevelInfo.checked == true ) then
			(
				print "Creating Level XML File"
				--create level xml data if checkbox is selected
				currFile = f_path + "\\" + maxFileName
				startIndx = findString currFile "."
				xmlFileName = replace currFile startIndx 4 ".xml"
				xmlFileHnd = createFile xmlFileName
				format "<?xml version=\"1.0\" encoding=\"iso-8859-1\" ?>\n" to:xmlFileHnd
				format "<LevelData>\n" to:xmlFileHnd
			)
		
		
		   if rdo1.state == 2 then
		   	o_arr = selection as array
		   else
		    o_arr = objects as array
			
		  clearSelection
		  destroyDialog XportTools
		  
		  for i=1 to o_arr.count do
		  (
			if ( expLevelInfo.checked == true ) then
			(
				print " Exporting Object: %\n" o_arr[i].name
				format "\t<Object>\n" to:xmlFileHnd				
				format "\t\t<type>TODO</type>\n" to:xmlFileHnd
				format "\t\t<ModelFileName>%.x</ModelFileName>\n" o_arr[i].name	to:xmlFileHnd
				format "\t\t<Pos>% % %</Pos>\n" o_arr[i].pos.x o_arr[i].pos.y o_arr[i].pos.z to:xmlFileHnd
				format "\t\t<Rotation>% % % %</Rotation>\n" o_arr[i].rotation.x o_arr[i].rotation.y o_arr[i].rotation.z o_arr[i].rotation.w to:xmlFileHnd
				format "\t\t<Scale>% % %</Scale>\n" o_arr[i].scale.x o_arr[i].scale.y o_arr[i].scale.z to:xmlFileHnd
				format "\t</Object>\n" to:xmlFileHnd
			)
		  	fnlx = f_path + "\\" + o_arr[i].name + ".x"			
			select o_arr[i]			
			if (i == 1 and chk1.checked == true) then
			(			
				 exportFile fnlx selectedOnly:true
			)
			else
			(
				exportFile fnlx #noPrompt selectedOnly:true
			)
		  )
		  --close the tags
			if ( expLevelInfo.checked == true ) then
			(
				format "</LevelData>\n" to:xmlFileHnd
			)		
		)
	)
		
	)	
	)	
	
	on execute do createDialog XportTools
	

	
)

