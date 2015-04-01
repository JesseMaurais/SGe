print "test6: exporting of C++ objects"

a = Account()

print "Making my first deposit of $4"
a:Deposit(4)

print "How much is in there?"
a:Print()

print "I got paid, deposit $50"
a:Deposit(50)

print "I want a burger for lunch, costs $10.50"
a:Withdrawl(10.50)

print "Whats left?"
a:Print()

print "I'm done at this bank!"

