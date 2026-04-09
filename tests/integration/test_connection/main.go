// Simple TCP listener to check for connection ok from a Clockwork client
package main

import (
	"fmt"
	"net"
	"os"
)

const PORT = 9090

func main() {
	listener, err := net.Listen("tcp", fmt.Sprintf(":%d", PORT))
	if err != nil {
		fmt.Println("Error listening:", err)
		os.Exit(1)
	}
	defer listener.Close()
	conn, err := listener.Accept()
	if err != nil {
		fmt.Println("Error accepting connection:", err)
		os.Exit(1)
	}
	conn.Close()
	os.Exit(0)
}
