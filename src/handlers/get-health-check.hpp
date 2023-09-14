#pragma once

#include <functional>

#include <drogon/drogon.h>

auto getHealthCheck()
{
    return [](const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        Json::Value json;
        json["message"] = "ok";
        auto resp       = drogon::HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    };
}
