# my-IRC-server

This is a second year Epitech project, it is therefore subjected to the "C Norm" wich is a programming style convention created by the school.  
The full epitech norm is available here: https://youtrack.jetbrains.com/_persistent/norme-en.pdf?file=74-190945&v=0&c=true
If some parts of the code seems weird in their syntax, it's probably due to this norm.

The purpose of this project was to create a IRC server and a simple test client.
* accept several simulataneous connections,
* be non-blocking and must absolutely implement the concept of a channel,
* comply with the RFC 1459 (Internet Relay Chat Protocol), its updates and dependencies.

```sh
Usage : ./server port
```

Forbidden functions :

* recv
* send
* all the system calls that make a socket non-blocking.


## Grade : 13.6/20
