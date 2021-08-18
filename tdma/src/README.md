Introduction                        {#mainpage}
============
OMNeT++ TDMA is a simple implementation to enable wireless nodes to use a TDMA MAC. 
It provides all modules to facilitate such a communication scenario:

- A MAC implementation, which confines transmissions to a scedule
- A scheduler implementation, which creates the transmission schedule
- A Rlc layer, which takes care of fragmentation/concatenation/reassembly

Getting started
============
To get started, just clone the repository, link it with INET and import it into your own project.

Testing
============
This project is has a suite of unit tests using gtest. They use a mock implementation of INET and OMNeT++ classes. See the `./test` folder for details.

Contributing
============
If you find a bug or have an idea for improvements, just get in touch with me at k.fuger@tuhh.de.