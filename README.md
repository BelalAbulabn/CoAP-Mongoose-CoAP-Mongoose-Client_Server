# CoAP Client and Server Example

This repository contains example code for a Constrained Application Protocol (CoAP) client and server. CoAP is a specialized web transfer protocol for use with constrained nodes and constrained networks in the Internet of Things. The CoAP client is written in C, and the CoAP server is written in Python.

## Getting Started

### Prerequisites

- For the CoAP client, you'll need a C compiler such as GCC.
- For the CoAP server, you'll need Python 3.x.

### Cloning the Repository

To clone the repository, use the following command:
git clone <repository-url>
  
Replace `<repository-url>` with the URL of this repository.

## CoAP Client

The CoAP client is implemented in C and uses the Mongoose and lwIP libraries. It can be used to send CoAP requests to a CoAP server.

### Building the Client

Navigate to the client directory and use the makefile to build the client.

```sh
cd client_directory
make
### Running the Client
After building, you can run the client using:

sh
./client_executable
