#include "server/LoginCommand.h"
#include "common/LoginRequest.h"

void LoginCommand::execute(std::shared_ptr<Session> session, const Packet& p) {
    LoginRequest loginReq = p.unpackBody<LoginRequest>();

    session->handleLogin(loginReq.userId, loginReq.nickname);
}
