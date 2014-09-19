#!./SGe
box = Geometry()
box:Box(2.0)
box:AngularVel(1.0, 0.0, 0.5)
while event.next do 
	if event.type == event.QUIT then
		print('will have already finished')
		break
	end
end
