// Project headers 
#include "ipc/tests.h"

// Default values
private const int self_conn_rx = 11;
private const int self_conn_tx = 22;

void test_client_api_connect()
{
  // Create placeholders for wrappers //
  
  // connect()
  const struct sockaddr_un address = {
    .sun_family = AF_UNIX,
    .sun_path   = "./socket.socket",
  };
  const socklen_t address_len = sizeof(address);

  // ipc_connect()
  const char name[] = "bob";

  // Configure wrappers //

  // self.conn.rx = socket()
  will_return(__wrap_socket, AF_UNIX);
  will_return(__wrap_socket, SOCK_STREAM | SOCK_NONBLOCK);
  will_return(__wrap_socket, 0);
  will_return(__wrap_socket, self_conn_rx);

  // self.conn.tx = socket()
  will_return(__wrap_socket, AF_UNIX);
  will_return(__wrap_socket, SOCK_STREAM);
  will_return(__wrap_socket, 0);
  will_return(__wrap_socket, self_conn_tx);

  // connect(self.conn.rx) 
  will_return(__wrap_connect, self_conn_rx);
  will_return(__wrap_connect, &address);
  will_return(__wrap_connect, address_len);
  will_return(__wrap_connect, 0);

  // write(name to host)
  will_return(__wrap_write, self_conn_rx);
  will_return(__wrap_write, name);
  will_return(__wrap_write, NAME_LEN);
  will_return(__wrap_write, 3);

  // connect(self.conn.tx)
  will_return(__wrap_connect, self_conn_tx);
  will_return(__wrap_connect, &address);
  will_return(__wrap_connect, address_len);
  will_return(__wrap_connect, 0);

  // write(name to host)
  will_return(__wrap_write, self_conn_tx);
  will_return(__wrap_write, name);
  will_return(__wrap_write, NAME_LEN);
  will_return(__wrap_write, 3);

  // Run ipc_connect function 
  assert_int_equal(ipc_connect((char *) name), 0);
}

void test_client_api_send()
{
  // Create placeholders for wrappers 
  const char dest[] = "pwr";
  const char msg[] = "turn on camera";
  const size_t msg_len = 14;

  const char expect_msg[] = "pwr turn on camera";
  const size_t expect_msg_len = 18;

  // Configure wrappers //

  // write(msg) to IPC 
  will_return(__wrap_write, self_conn_tx);
  will_return(__wrap_write, expect_msg);
  will_return(__wrap_write, expect_msg_len);
  will_return(__wrap_write, expect_msg_len);

  // Run ipc_send
  assert_int_equal(ipc_send((char *) dest, (char *) msg, msg_len), 0);
}

void test_client_api_recv()
{
  // Create placeholders for wrappers 
  const char msg_in[] = "pay take picture";
  const size_t msg_in_len = 16;

  const char expect_msg[] = "take picture";
  const size_t expect_msg_len = 12;

  // Configure wrappers //
  // read() from IPC 
  will_return(__wrap_read, self_conn_rx);
  will_return(__wrap_read, msg_in);
  will_return(__wrap_read, msg_in_len);
  will_return(__wrap_read, MAX_MSG_LEN);
  will_return(__wrap_read, msg_in_len);

  // Create placeholder for ipc_recv 
  char msg[MAX_MSG_LEN];

  // Run ipc_recv & check result
  assert_int_equal(ipc_recv("*", msg, MAX_MSG_LEN), expect_msg_len);
  assert_memory_equal(msg, expect_msg, expect_msg_len);
}