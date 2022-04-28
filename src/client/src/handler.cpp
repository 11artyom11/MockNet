#include "handler.h"

using Handler = Client::Handler;

Handler::Handler ()
{
    aes_shrd_ptr = std::make_unique<AES_Unit>();
    rsa_shrd_ptr = std::make_unique<RSA_Unit>();

}

void Handler::commap_init (void)
{
    commap[CONNECT_REQUEST]   = &Handler::send_connect_request;
    commap[LOG_IN_REQUEST]    = &Handler::send_login_request;
    commap[CONNECT_ACCEPT]    = &Handler::on_connect_accept_recieved;
    commap[LOG_IN_ACCEPT]     = &Handler::on_login_accept_recieved;
    commap[CONNECT_COMMAND]   = &Handler::send_connect_command;
    commap[LOG_IN_COMMAND]    = &Handler::send_login_command;
    commap[SIGN_UP_COMMAND]   = &Handler::send_sign_up_command;
    commap[EXIT_COMMAND]      = &Handler::send_terminate_connection;
    commap[CONNECT_VERIFY]    = &Handler::on_connect_verify_recieved;
    commap[LOG_IN_VERIFY]     = &Handler::on_login_verify_recieved;
    commap[SIGN_UP_VERIFY]    = &Handler::on_sign_up_verify_recieved;

}

int Handler::send_login_request(int sfd,
                                     const DataTransfer::MessageModel&)
{
 
}

int Handler::send_connect_request(int sfd, 
                                    const DataTransfer::MessageModel&)
{
    this->current_state = CONNECT_STATE::conn_request;
    DataTransfer::ConnectRequest cR{"127.0.0.1"};
    string to_send = cR.to_str();
    int res = send(sfd, (char*)to_send.c_str(), to_send.length(), NULL);
    return res;
}

int Handler::on_connect_accept_recieved(int sfd,
                                            const DataTransfer::MessageModel& message)
{

    Debug().info ("in on_connect_accept_recieved");
    /*
    Recieve RSA public key from server, cipher by it aes token and send 
    ecncrypted token to server
    */
    this->current_state = CONNECT_STATE::conn_accept;

   send_connect_command(sfd, message);
   
    return 0;

}   

int Handler::on_login_accept_recieved(int sfd,
                                        const DataTransfer::MessageModel&)
{

}

int Handler::send_connect_command(int sfd,
                                    const DataTransfer::MessageModel& message)
{
    
    Debug().info ("In send_connect_command");
    Debug().info("Message recieved  : ", message.to_str());
    
    RSA_Unit rsaU;

    auto key = "0123456789ABCDEF"; //aes_shrd_ptr->generate_key(16);
    auto utoken = message.get<string>("unique_token");
    string rsa_key =message.get<string>("pkey");
    Debug().warning ("KEY FETCHED : \n", rsa_key );
    rsaU.init_public_key ((unsigned char*)(rsa_key.c_str()));

    cP.AES_token = key;
    cP.unique_token = utoken;

    DataTransfer::ConnectCommand cC{"localhost",std::string{key},utoken};

    string raw_str = cC.to_str();//"{\"command\":\"com_connect\", \"ip\":\"127.0.0.1\", \"aes_token\":\"1234556789\", \"unique_token\":\""+cP.unique_token+"\"}";
    Debug().info ("RAW_STR : ", raw_str);
    int datalen = raw_str.length();
    unsigned char* encrypted = new unsigned char[1024];
    int enclen = rsaU.public_encrypt ((unsigned char*)(raw_str.c_str()), datalen, encrypted);

    Debug().info (enclen);
    
    send (sfd, (char*)encrypted, enclen, NULL);
    this->current_state = CONNECT_STATE::conn_commnd;
    Debug().info ("Ended send_connect_command");
    return 0;
}

int Handler::send_login_command(int sfd,
                                     const DataTransfer::MessageModel&)
{

}

int Handler::send_sign_up_command(int sfd, 
                                    const DataTransfer::MessageModel&)
{

}

int Handler::on_connect_verify_recieved(int sfd,
                                            const DataTransfer::MessageModel& message)
{   
    Debug().info ("in on_connect_verify_recieved ");
    Debug().info ("Message :",message.get<string>("unique_token"));
    string utoken = hex_to_string(message.get<string>("unique_token"));
    unsigned char* token_c = (unsigned char*)(utoken.c_str());
    unsigned char* key_ch = (unsigned char*)(cP.AES_token.c_str());
    Debug().info ("KEY : ", key_ch);
    unsigned char token_d[2048];
    int len = message.get<int> ("token_len");
    Debug().info ("LEN : ", len);
    aes_shrd_ptr->decrypt(token_c, len, key_ch, token_d);
    token_d[cP.unique_token.length()] = '\0';
    Debug().info ("INitial len : ", cP.unique_token.length());
    Debug().info ("Final message got: ", token_d);
    if (!cP.unique_token.compare((char*)token_d))
    {
        Debug().info ("CONNECTION VERIFIED");
        this->current_state = CONNECT_STATE::conn_verify;
    }
    else 
    {
        Debug().fatal ("Tokens donot match");
    }


    return 0;
}

int Handler::on_sign_up_verify_recieved(int sfd, 
                                            const DataTransfer::MessageModel&)
{

}

int Handler::on_login_verify_recieved(int sfd,
                                        const DataTransfer::MessageModel&)
{

}

int Handler::send_terminate_connection(int sfd,
                                        const DataTransfer::MessageModel&)
{

}

RSA_Unit_shrd_ptr
Handler::get_rsa_ptr (void) const
{
    return this->rsa_shrd_ptr;
}

AES_Unit_shrd_ptr
Handler::get_aes_ptr (void) const
{
    return this->aes_shrd_ptr;
}

CONNECT_STATE Handler::get_net_state(void) const
{
    return this->current_state;
}

ClientPrototype const* 
Handler::get_client_prototype_ptr_c (void) const
{
    return &(this->cP);
}


decltype(&Client::Handler::send_connect_request) 
Handler::get_command  ( std::string command)
{
    try
    {
        Debug().info("SIZE : ", commap.size());
        return commap.at(command);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return nullptr;
    }
}
