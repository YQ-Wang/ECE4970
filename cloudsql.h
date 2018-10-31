// Google Cloud SQL

#ifndef __CLOUDSQL_H__
#define __CLOUDSQL_H__

#define DATABASE_NAME		"scada"
#define DATABASE_IP         "35.192.121.176"
#define DATABASE_USERNAME	"root"
#define DATABASE_PASSWORD	"ece4970"

MYSQL *mysql1;

void mysql_connect(void)
{
    //initialize MYSQL object for connections
	mysql1 = mysql_init(NULL);

    if(mysql1 == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql1));
        return;
    }

    //Connect to the database
    if(mysql_real_connect(mysql1, DATABASE_IP, DATABASE_USERNAME, DATABASE_PASSWORD, DATABASE_NAME, 0, NULL, 0) == NULL)
    {
    	fprintf(stderr, "%s\n", mysql_error(mysql1));
    }
    else
    {
        printf("Database connection successful.\n");
    }
}

void mysql_disconnect(void)
{
    mysql_close(mysql1);
    printf( "Disconnected from database.\n");
}

#endif 