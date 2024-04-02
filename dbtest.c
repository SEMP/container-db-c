#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <libpq-fe.h>

#define DATABASE_HOST "DATABASE_HOST"
#define DATABASE_PORT "DATABASE_PORT"
#define DATABASE_NAME "DATABASE_NAME"
#define DATABASE_USER "DATABASE_USER"
#define DATABASE_PASSWORD "DATABASE_PASSWORD"
#define CONNECTION_STRING_SIZE 250

char * getVariable(char *name, char *defaultValue);

int main(int argc, char *argv[], char *envp[])
{
	printf("\n------------------------------\n");
	printf("Available environment variables:\n\n");
	
	for(int i = 0; envp[i] != NULL; i++)
	{
		printf("%s\n", envp[i]);
	}
	
	fflush(stdout);

	printf("\n------------------------------\n");
	
	char *host = getVariable(DATABASE_HOST, "test-postgres");
	char *port = getVariable(DATABASE_PORT, "5432");
	char *dbname = getVariable(DATABASE_NAME, "dvdrental");
	char *user = getVariable(DATABASE_USER, "postgres");
	char *password = getVariable(DATABASE_PASSWORD, "postgres");
	
	char connectionString[CONNECTION_STRING_SIZE];
	
	snprintf(connectionString, CONNECTION_STRING_SIZE, "host=%s port=%s dbname=%s user=%s password=%s", host, port, dbname, user, password);

	PGconn *conn = PQconnectdb(connectionString);
	
	if (PQstatus(conn) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	
	PGresult *res = PQexec(conn, "SELECT country FROM public.country;");
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		fprintf(stderr, "SELECT failed: %s\n", PQerrorMessage(conn));
		PQclear(res);
		PQfinish(conn);
		exit(1);
	}
	
	int rows = PQntuples(res);
	
	for(int i = 0; i < rows; i++)
	{
		printf("%s\n", PQgetvalue(res, i, 0));
	}
	
	PQclear(res);
	PQfinish(conn);
	
	return 0;
}

char * getVariable(char *name, char *defaultValue)
{
	char *value = getenv(name);
	
	if(value == NULL)
	{
		return defaultValue;
	}
	
	return value;
}
