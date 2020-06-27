#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <zmq.h>
#include <czmq.h>
#include <string.h>
#include <leveldb/db.h>

using namespace leveldb;

int main(int argc, char* argv[]) {
    srandom(time(NULL));
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <db name>\n";
        return 1;
    }

    Options options;
    options.create_if_missing = true;
    std::string name = argv[1];
    DB* dbptr;
    Status status = DB::Open(options, name, &dbptr);

    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        return 1;
    }
    std::unique_ptr<DB> db(dbptr);

    WriteOptions writeOptions;

    status = db->Put(writeOptions,
                     std::to_string(random()),
                     std::to_string(random()));
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        return 1;
    }

    ReadOptions readOptions;
    std::unique_ptr<Iterator> iter(db->NewIterator(readOptions));
    iter->SeekToFirst();
    while (iter->Valid()) {
        Slice key = iter->key();
        Slice value = iter->value();
        printf("%.*s => %.*s\n", (int) key.size(), key.data(),
               (int) value.size(), value.data());
        iter->Next();
    }

    
    zsock_t *asdf = zsock_new(ZMQ_REP);
    zsock_destroy(&asdf);
    void *context = zmq_ctx_new();
    void *sock = zmq_socket(context, ZMQ_DEALER);
    void *receiver = zmq_socket(context, ZMQ_DEALER);

    
    zmq_connect(sock, "tcp://localhost:8080");
    zmq_bind(receiver, "tcp://*:8080");

    char msg[] = "hello";
    zmq_send(sock, msg, strlen(msg), 0);

    char buf[1024];
    int len = zmq_recv(receiver, buf, sizeof(buf), 0);

    buf[len < sizeof(buf) ? len : sizeof(buf) - 1] = '\0';

    puts(buf);

    zmq_close(receiver);
    zmq_close(sock);
    zmq_ctx_destroy(context);
    return 0;
}
