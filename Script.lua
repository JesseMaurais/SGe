#!./SGe
box = Geometry()
box:Box(1.0)
box:AngularVel(1.0, 0.0, 0.5)
while event.next do 
	if event.type == event.QUIT then
		print('will now quit')
		break
	end
end

