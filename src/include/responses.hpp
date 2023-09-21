#pragma once

#include <drogon/drogon.h>

drogon::HttpResponsePtr buildHttpResponse(drogon::HttpStatusCode code)
{
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(code);
    return resp;
}

drogon::HttpResponsePtr makeResponseBadRequest() { return buildHttpResponse(drogon::k400BadRequest); }

drogon::HttpResponsePtr makeResponseNotFound() { return buildHttpResponse(drogon::k404NotFound); }

drogon::HttpResponsePtr makeResponseUnprocessableEntity() { return buildHttpResponse(drogon::k422UnprocessableEntity); }

drogon::HttpResponsePtr makeFailedResponse() { return buildHttpResponse(drogon::k500InternalServerError); }
