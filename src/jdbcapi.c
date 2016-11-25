#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "jni.h"
#include "jdbcapi.h"

struct jdbc_api_impl
{
	JDBC_API base;
	JNIEnv *env;
	jclass cls;
	JavaVMOption options[ 1 ];
	JavaVMInitArgs vm_args;
	JavaVM *jvm;
	jmethodID mid_get_connection;
	jmethodID mid_prepare_stmt;
	jmethodID mid_execute_query;
	jmethodID mid_set_column;
	jmethodID mid_read_value;
	jmethodID mid_log_off;

};
typedef struct jdbc_api_impl JDBC_API_IMPL;

static double read_value ( JDBC_API *obj,  int index_stmt)
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jint status;
	jint res;
	jdouble value;

	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return -1;
	}
	value = ( *api_impl->env )->CallStaticDoubleMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_read_value,
		index_stmt
		);
	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

	return value;

}

static bool get_connection (
	JDBC_API * obj,
	const char* ul,
	const char* userid,
	const char* pwd )
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jboolean jflag;
	jint status;
	jint res;

	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return -1;
	}
	jstring url = ( *api_impl->env )->NewStringUTF( api_impl->env, ul );
	jstring user = ( *api_impl->env )->NewStringUTF( api_impl->env, userid );
	jstring passwd = ( *api_impl->env )->NewStringUTF( api_impl->env, pwd );

	jflag = ( *api_impl->env )->CallStaticBooleanMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_get_connection,
		url,
		user,
		passwd );
	( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, url, 0 );
	( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, user, 0 );
	( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, passwd, 0 );
	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

	return jflag;
}

static int prepare_stmt ( JDBC_API * obj, const char *sql )
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jboolean jflag;
	jint res;
	jint index;
	jint status;

	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return false;
	}

	jstring query = ( *api_impl->env )->NewStringUTF( api_impl->env, sql );
	index = ( *api_impl->env )->CallStaticIntMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_prepare_stmt,
		query );
	( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, query, 0 );
	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

	return index;

}

static void set_column ( JDBC_API * obj, int id, const char *col_name )
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jint res;
	jint status;

#if 0
	status = ( *api_impl->jvm )->GetEnv(
		api_impl->jvm,
		(void**) &api_impl->env,
		api_impl->vm_args.version );
	if ( status != JNI_OK )
	{
		res = ( *api_impl->jvm )->AttachCurrentThread(
			api_impl->jvm,
			(void**) &api_impl->env,
			NULL );
		if ( res < 0 )
		{
			perror( "Attach the ENV failed\n" );
			return;
		}
		//const char* col_name = (char *) POINT_COLUMN( point_array[ j ] );
		jstring colname = ( *api_impl->env )->NewStringUTF(
			api_impl->env,
			col_name );
		printf( "set_column :%s\n", col_name );
		( *api_impl->env )->CallStaticVoidMethod(
			api_impl->env,
			api_impl->cls,
			api_impl->mid_set_column,
			id,
			colname );
		( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, colname, 0 );
		( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

	}
	else
	{
#endif
	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return;
	}
	jstring colname = ( *api_impl->env )->NewStringUTF(
		api_impl->env,
		col_name );
	printf( "set_column :%s\n", col_name );
	( *api_impl->env )->CallStaticVoidMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_set_column,
		id,
		colname );
	( *api_impl->env )->ReleaseStringUTFChars( api_impl->env, colname, 0 );
	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );
 //}

}

static bool execute_query ( JDBC_API * obj ,int index_stmt)
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jboolean jflag;
	jint res;
	jint status;

	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return false;
	}

	jflag = ( *api_impl->env )->CallStaticBooleanMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_execute_query ,
		index_stmt);
	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

	return jflag;

}

static void logoff ( JDBC_API * obj )
{
	JDBC_API_IMPL * api_impl = (JDBC_API_IMPL*) obj;
	jint res;
	jint status;

	res = ( *api_impl->jvm )->AttachCurrentThread(
		api_impl->jvm,
		(void**) &api_impl->env,
		NULL );
	if ( res < 0 )
	{
		perror( "Attach the ENV failed\n" );
		return;
	}
	( *api_impl->env )->CallStaticVoidMethod(
		api_impl->env,
		api_impl->cls,
		api_impl->mid_log_off );

	( *api_impl->jvm )->DetachCurrentThread( api_impl->jvm );

}

JDBC_API* jdbcapi_create ()
{
	long status;
	JDBC_API_IMPL * api_impl = apal_mem_alloc( sizeof(JDBC_API_IMPL) );
	RNON( api_impl );

	api_impl->options[ 0 ].optionString = "-Djava.class.path=.";
	memset( &api_impl->vm_args, 0, sizeof( api_impl->vm_args ) );
	api_impl->vm_args.version = JNI_VERSION_1_8;
	api_impl->vm_args.nOptions = 1;
	api_impl->vm_args.options = api_impl->options;

	status = JNI_CreateJavaVM(
		&api_impl->jvm,
		(void**) &api_impl->env,
		&api_impl->vm_args );
	if ( status != JNI_OK )
	{
		perror( "JVM Created failed!\n" );
		return NULL;
	}
	else
	{
		printf( "JVM Created success!\n" );
		api_impl->cls = ( *api_impl->env )->FindClass(
			api_impl->env,
			"DevJDBC" );
		if ( api_impl->cls != 0 )
		{
			api_impl->mid_get_connection =
				( *api_impl->env )->GetStaticMethodID(
					api_impl->env,
					api_impl->cls,
					"getConnection",
					"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z" );

			api_impl->mid_prepare_stmt = ( *api_impl->env )->GetStaticMethodID(
				api_impl->env,
				api_impl->cls,
				"prepareStmt",
				"(Ljava/lang/String;)I" );

			api_impl->mid_set_column = ( *api_impl->env )->GetStaticMethodID(
				api_impl->env,
				api_impl->cls,
				"setColname",
				"(ILjava/lang/String;)V" );

			api_impl->mid_execute_query = ( *api_impl->env )->GetStaticMethodID(
				api_impl->env,
				api_impl->cls,
				"executeQuery",
				"(I)Z" );
			api_impl->mid_read_value = ( *api_impl->env )->GetStaticMethodID(
				api_impl->env,
				api_impl->cls,
				"readValue",
				"(I)D" );
			api_impl->mid_log_off = ( *api_impl->env )->GetStaticMethodID(
				api_impl->env,
				api_impl->cls,
				"logoff",
				"()V" );
			api_impl->base.get_connection = get_connection;
			api_impl->base.prepare_stmt = prepare_stmt;
			api_impl->base.set_column = set_column;
			api_impl->base.execute_query = execute_query;
			api_impl->base.read_value = read_value;
			api_impl->base.logoff = logoff;
		}
		else
		{
			perror( "can not find the JDBC class\n" );
			return NULL;
		}
	}
	return (JDBC_API *) api_impl;
}

void jdbcapi_destroy ( JDBC_API * api_obj )
{
	//JDBC_API_IMPL* api_impl = (JDBC_API_IMPL *) api_obj;
	printf( "DevJDBC:%s\n", __func__ );
	if ( api_obj  != NULL )
	{
		free( api_obj );
	}

}

