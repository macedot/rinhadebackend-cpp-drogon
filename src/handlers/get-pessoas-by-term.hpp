#pragma once

#include <functional>

#include <drogon/drogon.h>

#include "../include/responses.hpp"
#include "../include/utils.hpp"

auto getPessoasByTerm()
{
    return [](const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&& callback,
              const std::string& search) {
        if (search.empty()) {
            callback(makeResponseBadRequest()); // "The query parameter 't' is required"
            return;
        }

        auto clientPtr = drogon::app().getDbClient();

        auto f = clientPtr->execSqlAsyncFuture(
            "SELECT people.id, people.nickname, people.name, people.birth_date, people.stack"
            " FROM people"
            " WHERE searchable LIKE $1 "
            " LIMIT 50;",
            "%" + str_tolower(search) + "%");

        try {
            Json::Value json   = Json::arrayValue;
            int         i      = 0;
            auto        result = f.get();

            for (auto row : result) {
                json[i]["id"]         = result[i]["id"].as<std::string>();
                json[i]["apelido"]    = result[i]["nickname"].as<std::string>();
                json[i]["nome"]       = result[i]["name"].as<std::string>();
                json[i]["nascimento"] = result[i]["birth_date"].as<std::string>();
                splitValues(json[i]["stack"], result[i]["stack"].as<std::string>());
                i++;
            }

            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            callback(resp);
        }
        catch (const drogon::orm::DrogonDbException& e) {
            logException(e.base().what());
            callback(makeFailedResponse());
        }

        return;
    };
}
