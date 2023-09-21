#pragma once

#include <functional>

#include <drogon/drogon.h>

#include "../include/responses.hpp"
#include "../include/utils.hpp"

auto getPessoasById()
{
    return [](const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&& callback,
              const std::string& id) {
        if (id.size() != 36) {
            callback(makeResponseNotFound()); // "This person do not exist."
            return;
        }

        auto clientPtr = drogon::app().getDbClient();

        auto f = clientPtr->execSqlAsyncFuture(
            "SELECT people.id, people.nickname, people.name, people.birth_date, people.stack"
            " FROM people"
            " WHERE people.id = $1;",
            id);

        try {
            auto result = f.get();
            if (result.empty()) {
                callback(makeResponseNotFound()); // "This person do not exist."
                return;
            }

            Json::Value json;
            json["id"]         = result[0]["id"].as<std::string>();
            json["apelido"]    = result[0]["nickname"].as<std::string>();
            json["nome"]       = result[0]["name"].as<std::string>();
            json["nascimento"] = result[0]["birth_date"].as<std::string>();
            splitValues(json["stack"], result[0]["stack"].as<std::string>());

            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            callback(resp);
        }
        catch (const drogon::orm::DrogonDbException& e) {
            logException(e.base().what());
            callback(makeFailedResponse());
        }
    };
}
