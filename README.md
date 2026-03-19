## UML class diagram
```mermaid
 classDiagram
    %% RELATION DEFINITIONS
    Room <|-- PublicRoom
    Room <|-- PrivateRoom
    DBConnector <|-- MSSQLConnector
    DBConnector <|-- SQLiteConnector
    Command <|-- JoinCommand
    Command <|-- LeaveCommand
    Command <|-- ChatMessageCommand
    Command <|-- ListRoomCommand

    %% COMMON CLASSES
    namespace COMMON {
        class Packet {}
        class User{}
    }

    %% SERVER CLASSES
    namespace SERVER {
        class Server {}
        class Session{}
        class RoomManager{}
        class Room{}
        class PublicRoom {}
        class PrivateRoom {}
        class Command{}
        class JoinCommand{}
        class LeaveCommand{}
        class ChatMessageCommand{}
        class ListRoomCommand{}
        class DBConnector{}
        class MSSQLConnector {}
        class SQLiteConnector {}
    }

    %% CLIENT CLASSES
    namespace CLIENT {
        class NetworkManager{}
        class AudioManager{}
        class AudioCodec{}
    }
    


