#pragma once

#include <functional>

#include <drogon/drogon.h>

#include "../include/responses.hpp"
#include "../include/utils.hpp"

auto getContagemPessoas()
{
    return [](const drogon::HttpRequestPtr&, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        auto clientPtr = drogon::app().getDbClient();
        auto f         = clientPtr->execSqlAsyncFuture("SELECT COUNT(*) AS count FROM people;");

        try {
            auto        result = f.get();
            Json::Value json   = Json::objectValue;
            json["count"]      = result.empty() ? 0 : result[0]["count"].as<int>();
            auto resp          = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::HttpStatusCode::k200OK);
            callback(resp);
        }
        catch (const drogon::orm::DrogonDbException& e) {
            logException(e.base().what());
            callback(makeFailedResponse());
        }
    };
}
