// Desafio técnico para Projetista de Sistemas Embracados - MOBIT 14/06/2022
// Candidato: Herivelton
//2. Sincronizando o uso do log com várias threads

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <syslog.h>

std::mutex mtx;
static const int THREAD_COUNT = 3;

static void print_log(int id)
{
    mtx.lock();
    syslog (LOG_WARNING , "Lock inicial da Thread %d", id);
    std::cout << "--------------------" <<'\n';
    std::cout << "Iniciando bloco " << id << '\n';
    std::cout << "Hello world from thread " << id << '\n';
    std::cout << "Fim do bloco " << id << '\n';
    syslog (LOG_WARNING , "Teste de envio para syslog - Thread %d", id);
    std::cout << "--------------------" << '\n';
    syslog (LOG_WARNING , "Unlock no final da Thread %d", id);
    mtx.unlock();
}

int main()
{
    std::vector<std::thread> v;
    setlogmask (LOG_UPTO (LOG_NOTICE));

    openlog ("MYTESTPROGRAM", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        v.emplace_back(print_log, i);

    }

    for (auto &t : v)
    {
        t.join();
    }

    closelog ();

    return 0;
}
