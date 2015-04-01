print('test3: using C structures for data storage')

a = Record()
print('a has type ' .. tostring(a))
a.byte = 32
a.word = 30000
a.real = 3.14
print('a.byte = ' .. a.byte)
print('a.word = ' .. a.word)
print('a.real = ' .. a.real)

b = Record{byte=1, real=2.71, word=3}
print('b has type ' .. tostring(b))
print('b.byte = ' .. b.byte)
print('b.word = ' .. b.word)
print('b.real = ' .. b.real)

recvec = Record(10)
print('recvec is a vector of records with ' .. #recvec .. ' items')
b = recvec[5]
print('b = recvec[5] is a ' .. tostring(b) .. ' with #b = ' .. #b)
b.byte = 8
print('b.byte = ' .. b.byte .. ' makes recvec[5].byte = ' .. recvec[5].byte)

-- Test that invalid keys throw an error

print('You should get std::out_of_range from C++')
a.bogus = nil
error('Should not get here')
