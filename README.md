# OOT_2_Hash

Small-scope implementation of an SQLite3 Input wrapper is included
- This has hinted at possible issues involving performance - Possible Solutions: 
	- Look into using it with syncronous=OFF
	- Look into simply doing the message writing in a seperate thread (this may be simple as there is no risk of collision)
	- Storing the data as a series of queries to send to the DB, rather than quickly sending multiple queries
		- Implementation of a small-scale buffer may be necessary & as only a small amount of data is being held per packet (int&float), it's not a hug memory over-head

Also: Write the fucking program as well