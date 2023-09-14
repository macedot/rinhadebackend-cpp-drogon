CREATE TABLE IF NOT EXISTS people (
    id uuid PRIMARY KEY NOT NULL,
    nickname VARCHAR(32) UNIQUE NOT NULL,
    "name" VARCHAR(100) NOT NULL,
    birth_date DATE NOT NULL,
    stack TEXT NULL,
    searchable TEXT GENERATED ALWAYS AS (
        LOWER(name || ' ' || nickname || ' ' || stack)
    ) STORED
);

CREATE EXTENSION IF NOT EXISTS PG_TRGM;

CREATE INDEX CONCURRENTLY IF NOT EXISTS IDX_PEOPLE_BUSCA_TGRM
    ON public.people
    USING GIST (searchable GIST_TRGM_OPS(SIGLEN = 128));
