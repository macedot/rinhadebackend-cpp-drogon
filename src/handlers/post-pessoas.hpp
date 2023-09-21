#pragma once

#include <functional>
#include <string>

#include <fmt/format.h>

#include <drogon/drogon.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_io.hpp>         // streaming operators
#include <boost/uuid/uuid_generators.hpp> // generators

#include "../include/responses.hpp"
#include "../include/utils.hpp"

auto postPessoas()
{
    return [](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        const auto jsonPtr = req->jsonObject();
        if (!jsonPtr) {
            callback(makeFailedResponse());
            return;
        }

        const auto nicknameValue = jsonPtr->get("apelido", "");
        if (nicknameValue.isNull() || !nicknameValue.isString()) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        const auto nickname = nicknameValue.asString();
        if (nickname.empty() || nickname.size() > 32) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        auto clientPtr   = drogon::app().getDbClient();
        auto personFound = clientPtr->execSqlAsyncFuture("SELECT id FROM people WHERE people.nickname = $1;", nickname);
        try {
            auto result = personFound.get();
            if (!result.empty()) {
                callback(makeResponseUnprocessableEntity()); //"This person already exists."
                return;
            }
        }
        catch (const drogon::orm::DrogonDbException& e) {
            logException(e.base().what());
            callback(makeFailedResponse());
            return;
        }

        const auto nameValue = jsonPtr->get("nome", "");
        if (nameValue.isNull() || !nameValue.isString()) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        const auto name = nameValue.asString();
        if (name.empty() || name.size() > 100) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        const auto birthDateValue = jsonPtr->get("nascimento", "");
        if (birthDateValue.isNull() || !birthDateValue.isString()) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        const auto birthDate = birthDateValue.asString();
        if (birthDate.empty() || birthDate.size() != 10 || !isDateValid(birthDate)) {
            callback(makeResponseUnprocessableEntity());
            return;
        }

        Json::String elem;
        std::string  stacks;
        const auto   stacksJson = jsonPtr->get("stack", "");
        if (!stacksJson.isNull() && !stacksJson.empty() && stacksJson.isArray()) {
            if (stacksJson[0].isNull() || !stacksJson[0].isString()) {
                callback(makeResponseUnprocessableEntity());
                return;
            }
            stacks = stacksJson[0].asString();
            if (stacks.size() > 32) {
                callback(makeResponseUnprocessableEntity());
                return;
            }
            for (int i = 1; i < int(stacksJson.size()); ++i) {
                if (stacksJson[i].isNull() || !stacksJson[i].isString()) {
                    callback(makeResponseUnprocessableEntity());
                    return;
                }
                elem = stacksJson[i].asString();
                if (elem.size() > 32) {
                    callback(makeResponseUnprocessableEntity());
                    return;
                }
                stacks += "," + elem;
            }
        }

        const std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());

        auto f = clientPtr->execSqlAsyncFuture(
            "INSERT INTO people (id, nickname, name, birth_date, stack)"
            " VALUES ($1, $2, $3, $4, $5)"
            // " ON CONFLICT DO NOTHING"
            " RETURNING id;",
            uuid, nickname, name, birthDate, stacks);

        std::string id;

        try {
            auto result = f.get();
            if (result.empty()) {
                callback(makeFailedResponse());
                return;
            }
            id = result[0]["id"].as<std::string>();
        }
        catch (const drogon::orm::DrogonDbException& e) {
            logException(e.base().what());
            callback(makeFailedResponse());
            return;
        }

        auto resp = buildHttpResponse(drogon::HttpStatusCode::k201Created);
        resp->addHeader("Location", std::string{"/pessoas/"} + id);
        callback(resp);
    };
}
