# [nome da aplicacao] 

## Sobre a aplicacao


### Instalacao (usuario)

Faca o download da aplicacao 

    git clone [falta atualizar]

E rode o algoritmo

    ./[nome da aplicacao]

### Instalacao (desenvolvedor)

Primeiramente, instale as dependencias

Deve-se possuir as bibliotecas [libc6-dev](https://packages.debian.org/search?keywords=libc6-dev), [gcc](https://packages.debian.org/search?keywords=gcc), [g++](https://packages.debian.org/search?keywords=g%2B%2B), [make](https://packages.debian.org/jessie/make) e [dpkg](https://packages.debian.org/search?keywords=dpkg) que podem ser instaladas atraves do pacote build-essential.

    sudo apt-get install build-essential 

Agora, dirija-se ao [repositorio oficial do OpenTLS](https://github.com/openssl/openssl) e instale tambem suas dependencias.

Apos efetuar a clonagem deste repositorio, nao esqueca de efetuar corretamente a compilacao dos arquivos .cpp usando o [C++14](https://en.wikipedia.org/wiki/C%2B%2B14) atraves do [g++](https://packages.debian.org/search?keywords=g%2B%2B).

    g++ -std=c++14 [nome da aplicacao].cpp [falta adiconar as flags] -o [nome da aplicacao]



## Seguranca e protocolos

Nossa aplicacao usa o [protocolo](https://en.wikipedia.org/wiki/List_of_RFCs) [TLS](https://tools.ietf.org/html/rfc5246) para se comunicar atraves da rede a fim de evitar [espionagem](https://en.wikipedia.org/wiki/Eavesdropping) e [alteracao de dados](https://en.wikipedia.org/wiki/Tamper-evident_technology).

Uma vez que os sistemas finais concordem no uso do conexoes [TLS](https://tools.ietf.org/html/rfc5246), sera negociada uma conexao com estado usando um procedimento de [handshaking](https://en.wikipedia.org/wiki/Handshaking). Nosso protocolo inicia com uma troca de [chave assimetricas](https://en.wikipedia.org/wiki/Public-key_cryptography) visando inciar uma conversa segura que estabelece as configuracoes das [cifras simetricas](https://en.wikipedia.org/wiki/Symmetric-key_algorithm) que serao usadas durante as outras mensagens. 

### Como funciona?

Um [handshaking](https://en.wikipedia.org/wiki/Handshaking) comeca quando um [cliente](https://en.wikipedia.org/wiki/Client_(computing)) tenta conectar ao nosso [servidor](https://en.wikipedia.org/wiki/Server_(computing)) por meio de uma conexao segura. Entao, como nosso [servidor](https://en.wikipedia.org/wiki/Server_(computing)) permite conexoes [TLS](https://tools.ietf.org/html/rfc5246), aceitamos a conexao caso seja possivel e geramos uma identificao na forma de [certificado digital](https://en.wikipedia.org/wiki/Public_key_certificate). Este certificado contem o nome do [servidor](https://en.wikipedia.org/wiki/Server_(computing)), uma assinatura autenticada que confirma a autenticidade do nosso certificado, e a chave criptografica publica do [servidor](https://en.wikipedia.org/wiki/Server_(computing)). 

Apos a validacao do certificado pelo [cliente](https://en.wikipedia.org/wiki/Client_(computing)), sera gerada pelo mesmo as chaves de sessao usadas para estabelecer a conexao, neste momento ha duas possiblidades: (1) o [cliente](https://en.wikipedia.org/wiki/Client_(computing)) [criptografa](https://en.wikipedia.org/wiki/Cryptography) um numero aleatorio com a chave publica do [servidor](https://en.wikipedia.org/wiki/Server_(computing)) e envia o resultado para o [servidor](https://en.wikipedia.org/wiki/Server_(computing)); ambos usando este numero aleatorio para gerar a chave unica de sessao que serve tanto para criptografia quanto para descriptografia ou (2) usa o [algoritmo de Diffie–Hellman](https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange) para gerar de forma segura uma chave de sessao unica que possui a propriedade de que, se a chave do [servidor](https://en.wikipedia.org/wiki/Server_(computing)) e descoberta no futuro, ela nao pode ser usada para descriptografar a sessao atual, mesmo que a sessao tenha sido interceptada completamente.

Uma vez que o [handshaking](https://en.wikipedia.org/wiki/Handshaking) e concluido, sera iniciada uma conexao segura, em que todo dado e [encriptado](https://en.wikipedia.org/wiki/Cryptography) e [desencriptado](https://en.wikipedia.org/wiki/Cryptography) usando a chave de sessao gerada ate o fechamento da conexao. 

> se algum dos procedimentos descritos falha, entao nenhuma conexao sera criada.

## Licença


