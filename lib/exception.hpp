#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>
#include <string>

/// Abstrai qualquer exceção lançada por qualquer objeto que abstrai uma fonte de imagens.
class IOException : public std::runtime_error
{
    public:

        /// Construtor.
        /**
        * @param what Mensagem de erro.
        */
        IOException(std::string const& what)
            : std::runtime_error(what)
        {}
};

/// Exceção lançada quando ocorre falha na interpretação do endereço da fonte de imagens.
class InvalidHostAddress : public IOException
{
    public:
        /// Construtor.
        /**
        * @param hostAddress endereço mal formado.
        */
        InvalidHostAddress(std::string const& hostAddress)
                    : IOException("Malformed host address " + hostAddress)
        {}
};

/// Exceção lançada quando conexão com uma fonte de imagens falha.
class HostUnreachable : public IOException
{
    public:
        /// Construtor.
        /**
        * @param hostAddress endereço da fonte de imagens com a qual a conexão falhou.
        */
        HostUnreachable(std::string const& hostAddress)
                    : IOException("Failed to connect to " + hostAddress)
        {}
};

/// Exceção lançada quando a leitura de dados de uma fonte de imagens falha.
class BrokenPipe : public IOException
{
    public:
        /// Construtor.
        BrokenPipe()
                    : IOException("IO error")
        {}
};

/// Exceção lançada quando o stream de dados chega ao fim.
class EndOfFile : public IOException
{
    public:
            /// Construtor.
            EndOfFile()
                    : IOException("eof")
            {}
};

#endif // EXCEPTION_HPP
