## UML class diagram
```mermaid
 classDiagram
    %% RELATION DEFINITIONS
    %% INHERITANCE
    Room <|-- PublicRoom
    Room <|-- PrivateRoom
    DBConnector <|-- MSSQLConnector
    DBConnector <|-- SQLiteConnector
    Command <|-- JoinCommand
    Command <|-- LeaveCommand
    Command <|-- ChatMessageCommand
    Command <|-- ListRoomCommand
    AudioCodec <|-- OpusCodec

    %% OTHER
    Server "1" *-- "1" RoomManager : has
    Server "1" *-- "1" DBConnector : uses
    RoomManager "1" *-- "*" Room : manages
    Room "1" o-- "*" Session : aggregates active
    Room "*" -- "*" User : has admins
    Session "1" -- "0..1" User : represents active
    Session ..> Command : creates and executes
    AudioManager "1" *-- "1" AudioCodec : uses


    %% COMMON CLASSES
    namespace COMMON {
        class Packet {
            -PacketHeader header
            -vector~char~ body
            +pack()
            +unpack()
        }
        class Message{
            +int senderId
            +int receiverId
            +string content
            +string timestamp
            +bool isAudio
        }
        class User{
            -int id
            -string nickname
            +getId()
            +getNickname()
        }
    }

    %% SERVER CLASSES
    namespace SERVER {
        class Server {
            -asio::io_context io_context
            -tcp::acceptor acceptor
            -RoomManager roomManager
            -DBConnector db
            +start()
            -start_accept()
        }
        class Session{
            -tcp::socket socket
            -shared_ptr~User~ user
            +do_read()
            +deliver(Packet p)
            +handlePacket(Packet p)
        }
        class RoomManager{
            -map~string, shared_ptr~Room~~ allRooms
            +createRoom(string name, bool isPrivate, shared_ptr~User~ owner, bool ownerIsAdmin)
            +getRoom(string name) shared_ptr~Room~
            +removeRoom(string name)
        }
        class Room{
            <<abstract>>
            #string name
            #vector~shared_ptr~Session~~ clients
            #vector~shared_ptr~User~~ admins
            +virtual canJoin(string token)* bool
            +broadcast(Packet p)
            +addClient(shared_ptr~Session~ s)
            +removeClient(shared_ptr~Session~ s)

        }
        class PublicRoom {
            +canJoin(string token) bool
        }
        class PrivateRoom {
            -string accessCode
            +canJoin(string token) bool
        }
        class Command{
            <<interface>>
            +execute(shared_ptr~Session~ s, vector~char~ b)*
        }
        class JoinCommand{
            +execute(shared_ptr~Session~ s, vector~char~ b)
        }
        class LeaveCommand{
            +execute(shared_ptr~Session~ s, vector~char~ b)
        }
        class ChatMessageCommand{
            +execute(shared_ptr~Session~ s, vector~char~ b)
        }
        class ListRoomCommand{
            +execute(shared_ptr~Session~ s, vector~char~ b)
        }
        class DBConnector{
            <<interface>>
            +virtual openConnection()
            +virtual authenticateUser(string nickname, string password)* bool
            +virtual saveMessage(Message m)*
            +virtual getHistory(int roomId)* vector~Message~
            +virtual getHistory(string nickname)* vector~Message~
        }
        class MSSQLConnector {
            +openConnection() bool
            +getHistory(int roomId) vector~Message~
            +getHistory(string nickname) vector~Message~
        }
        class SQLiteConnector {
            +openConnection() bool
            +getHistory(int roomId) vector~Message~
            +getHistory(string nickname) vector~Message~
        }
    }

    %% CLIENT CLASSES
    namespace CLIENT {
        class NetworkManager{
            -asio::io_context ctx
            -tcp::socket socket
            -thread workerThread
            +connect(string host, int port)
            +send(Packet p)
            +signal: messageReceived(Packet p)
        }
        class AudioManager{
            -AudioCodec codec
            +startRecording()
            +stopRecording()
            +playAudio(vector~char~ data)
        }
        class AudioCodec{
            <<interface>>
            +virtual encode(vector~float~ pcm)* vector~char~
            +virtual decode(vector~char~ data)* vector~float~
        }
        class OpusCodec{
            +encode(vector~float~ pcm)* vector~char~
            +decode(vector~char~ data)* vector~float~
        }
    }
```

## Database Entity Relationship diagram
```mermaid
erDiagram
USERS ||--o{ USERS_ROOMS : "belongs to"
    ROOMS ||--o{ USERS_ROOMS : "has members"
    USERS ||--o{ MESSAGES : "sends"
    ROOMS ||--o{ MESSAGES : "contains"
    USERS ||--o{ ROOMS : "owns (creator)"

    USERS {
        int id PK
        string nickname
        string password_hash
        datetime created_at
    }
    ROOMS {
        int id PK
        string name
        string type
        string access_code
        int owner_id FK
        datetime created_at
    }
    MESSAGES {
        int id PK
        int room_id FK
        int sender_id FK
        text content
        string audioURL
        datetime timestamp
        bool type
    }
    USERS_ROOMS {
        int id PK
        int room_id FK
        int user_id FK
        bool is_admin
    }
```
