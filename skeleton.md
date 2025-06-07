vector<uint8_t>& clientToServer;
vector<uint8_t>& ServerToClient;

serverProtocolMock {
	vector<uint8_t>& toServer; // poppeo lo que el client pusheo
	vector<uint8_t>& toClient; // pusheo lo que le respondo al client
}

clientProtocolMock {
	vector<uint8_t>& toServer; // pusheo lo que quiero mandar al server
	vector<uint8_t>& toClient; // poppeo lo que el server pusheo
}

serverProtocolMock(clientToServer, ServerToClient);
clientProtocolMock(clientToServer, ServerToClient);

