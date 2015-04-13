#!./SGe
XML('Config.xml')
while event.next do 
	if event.type == event.QUIT then
		print('will have already finished')
		break
	end
end
