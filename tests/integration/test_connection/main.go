// Simple TCP listener to check for connection ok from a Clockwork client
package main

import (
	"bytes"
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
	// read exactly one message from the connection
	message := make([]byte, 256)
	n, err := conn.Read(message)
	if err != nil {
		fmt.Println("Error reading message:", err)
		os.Exit(1)
	}
	fmt.Println("Message:", message[:n])
	if !bytes.Equal(message[:n], []byte{0, 8, 1, 0, 0, 0, 0, 0, 0, 0}) {
		fmt.Println("Message mismatch:", message[:n])
		os.Exit(1)
	}
	conn.Close()
	os.Exit(0)
}
