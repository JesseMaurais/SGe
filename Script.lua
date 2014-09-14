#!./SGe
box = Box()
box:AngularVel(1.0, 0.0, 0.5)
while event.next do 
	if event.type == event.QUIT then
		print('will now quit')
		break
	end
end

