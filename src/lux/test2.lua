print('test2: using C arrays for data storage')

a = int(10)

print('object a is '..tostring(a)..' and #a = '..#a)

for i = 0, #a-1 do
	a[i] = 2^i
end

for i = 0, #a-1 do
	print('a['..i..'] = '..a[i])
end

b = a + 5

print('b = a + 5 is '..tostring(b)..' and #b = '..#b)

for i = 0, 4 do
	print('b['..i..'] = '..b[i])
end

c = double(10)

for i = 0, #c-1 do
	c[i] = 1/a[i]
end

print('object c is '..tostring(c)..' and #c = '..#c)

for i = 0, #c-1 do
	print('c['..i..'] = '..c[i])
end

d = char('Hello World!')

print('object d is '..tostring(d)..' and #d = '..#d)

for i = 0, #d-1 do
	print('d['..i..'] = '..string.char(d[i]))
end

e = short{7, 10, 6, 14}

print('object e is '..tostring(e)..' and #e = '..#e)

for i = 0, 3 do
	print('e['..i..'] = '..e[i])
end



