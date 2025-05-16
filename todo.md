## TO DO TAI

- [X] Terminar del lado del client los manejadores
- [ ] Usar polimorfismo para desacoplar los mensajes. 

el tema es que deberia recibir el socket por referencia en la función polimorfica para enviar (me hace ruido)

en enviar¿

- [ ] Agregar manejador y parseador del lado del server
    - los maps que tengo ahora en commonProtocol deberian estar capaz en un commonParser para reutilzar esos mapas
- [ ] Agregar queues:

    - server sender: snapshots
    - client reciever: snapshots
    - server receiver: command (idk if i have made the struct or i may create a new one)
    - client sender: command

    - Del lado del client ambas deberian ser threads porque el hilo principal se encarga de la UI
    - Del lado del server tmb voy a lanzar dos hilos pq necesito el sender (este puede ser el mismo q el clientHandler original (quien se ocupa del lobby)), el receiver (uno nuevo), y ademas cuando arranca la partida necesitaria que ella sea un propio hilo (debería hacerlo facu ya esto)
