#include <drogon/drogon.h>

#include <fmt/format.h>

#include "include/responses.hpp"
#include "include/utils.hpp"

#include "handlers/get-contagem-pessoas.hpp"
#include "handlers/get-health-check.hpp"
#include "handlers/get-pessoas-by-id.hpp"
#include "handlers/get-pessoas-by-term.hpp"
#include "handlers/post-pessoas.hpp"

auto getEnvVar(const char* var) -> const char*
{
    if (auto value = std::getenv(var)) {
        return value;
    }
    throw std::runtime_error(fmt::format("Missing environment variable: {}", var));
}

int main()
{
    using namespace drogon;

    const auto port             = atoi(getEnvVar("PORT"));
    const auto dbMaxConnections = atol(getEnvVar("DB_MAX_CONNECTIONS"));
    const auto numThreads       = atoi(getEnvVar("NUM_THREADS"));
    const auto dbHost           = getEnvVar("DB_HOST");
    const auto dbPort           = atoi(getEnvVar("DB_PORT"));
    const auto dbName           = getEnvVar("DB_NAME");
    const auto dbUser           = getEnvVar("DB_USER");
    const auto dbPassword       = getEnvVar("DB_PASSWORD");

    logInfo("Starting rinhadebackend-cpp-drogon server on http://localhost:" + std::to_string(port));

    app().addListener("0.0.0.0", port).setThreadNum(numThreads).setIdleConnectionTimeout(10000);

    app().createDbClient("postgresql", dbHost, dbPort, dbName, dbUser, dbPassword, dbMaxConnections);

    app()
        .registerHandler("/health-check", getHealthCheck())
        .registerHandler("/pessoas", postPessoas(), {Post})
        .registerHandler("/pessoas/{id}", getPessoasById(), {Get})
        .registerHandler("/pessoas?t={search}", getPessoasByTerm(), {Get})
        .registerHandler("/contagem-pessoas", getContagemPessoas(), {Get})
        .run();

    return 0;
}
