// Simple TCP listener to check for connection ok from a Clockwork client
package main

import (
	"bytes"
	"fmt"
	"io"
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
	// set to nodelay mode to prevent nagle algorithm from interfering
	err = conn.(*net.TCPConn).SetNoDelay(true)
	if err != nil {
		fmt.Println("Error setting nodelay:", err)
		os.Exit(1)
	}
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
	if !bytes.Equal(message[:n], []byte{0, 8, 1, 0, 0, 0, 0, 0, 0, 0}) {
		fmt.Println("Message mismatch:", message[:n])
		os.Exit(1)
	}

	// now send a tick message
	message = make([]byte, 256)
	message[0] = 1
	message[1] = 0
	n, err = conn.Write(message[:2])
	if err != nil {
		fmt.Println("Error sending message:", err)
		os.Exit(1)
	}

	// now check for a graceful client disconnect
	_, err = conn.Read(message)
	if err == nil || err != io.EOF {
		fmt.Println("Expected graceful client disconnect, got:", err)
		os.Exit(1)
	}

	os.Exit(0)
}
