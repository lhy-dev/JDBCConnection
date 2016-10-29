#ifndef JDBCAPI_H_
#define JDBCAPI_H_

#include <stdbool.h>

typedef struct jdbc_api JDBC_API;

struct jdbc_api
{
	bool (*get_connection) (
		JDBC_API *obj,
		const char* url,
		const char* user,
		const char* passwd );
	int (*prepare_stmt) ( JDBC_API* obj, const char* sql );
	bool (*execute_query) ( JDBC_API* obj , int index_stmt);
	double (*read_value) ( JDBC_API* obj, int index_stmt);
	void (*set_column) ( JDBC_API* obj, int id, const char *col_name );
	void (*logoff) ( JDBC_API* obj );
};

JDBC_API *jdbcapi_create();

void jdbcapi_destroy( JDBC_API* api_obj );

#endif /* JDBCAPI_H_ */
