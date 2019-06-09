#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>  
#include <time.h>  
#include <unistd.h>  
#include <libmemcached/memcached.h>   

#define TOTAL_NUM  10000

/********************************
使用libmemcached的基本流程：
        1）创建memcached_st结构；
        2）添加memcached server；
        3）设置libmemcached库的一些属性（hash算法，重试次数及重试时间等）；
        4）调用基本的API（如get，set等）...；
        5）释放memcached_st结构；
*********************************/
  
int main(int argc, char *argv[])   
{   
  memcached_st *memc;   
  memcached_return rc;   
  memcached_server_st *servers; //服务器列表
  
  //connect multi server   
  memc = memcached_create(NULL);   
  
  
  //增加memcached 服务器到服务列表
  //memcached_server_list_append_with_weight(servers,  (char*)"localhost",11212,weight,&rc);
  servers = memcached_server_list_append(NULL, (char*)"localhost", 11211, &rc);   
  servers = memcached_server_list_append(servers, (char*)"localhost",11212, &rc);   
  servers = memcached_server_list_append(servers, (char*)"localhost",11213, &rc);   
  
  rc = memcached_server_push(memc, servers);  
  memcached_server_free(servers);    
   
  //MEMCACHED_DISTRIBUTION_MODULA   普通取模算法
  //MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA_SPY   Ketema 哈希算法
  memcached_behavior_set(memc,MEMCACHED_BEHAVIOR_DISTRIBUTION,MEMCACHED_DISTRIBUTION_CONSISTENT);  
  memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, 20) ;  
  //  memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_REMOVE_FAILED_SERVERS, 1) ;  // 同时设置MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT 和 MEMCACHED_BEHAVIOR_AUTO_EJECT_HOSTS  
  memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT, 5) ;  
  memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_AUTO_EJECT_HOSTS, true) ;  //设置自动隔离已离线的服务器
     
  int times = 0 ;
  char key[16];
  size_t key_len = 0;
 
      
  int i = 0;  
  for (; i < TOTAL_NUM; i++)        
  { 
  	 //写memcached
     {
          int len = 0;
          snprintf(key,16,"key%d",i);
          len = strlen(key);
          rc = memcached_set(memc, key,key_len , key, key_len, (time_t)18000,(uint32_t)0);
	        if(rc == MEMCACHED_SUCCESS){
              printf("key: %s  rc:%s\n", key, memcached_strerror(memc, rc));   // 输出状态 
          }else{
              fprintf(stderr,"Error! set key %s  failed.",key);
          }
              
      }
      	
      //读memcached  
      {
         	char * result_str; //查询结果
         	size_t result_str_length = 0;
         	uint32_t  flags;
          result_str= memcached_get(memc, key, key_len,&result_str_length, &flags, &rc);

          if (rc == MEMCACHED_SUCCESS) 
          {
              printf("%s\n",result_str);
              free(result_str); /*一定要free掉 memcached传回的数据*/
          }else {
              fprintf(stderr,"get key %s failed.",key);
          }
         	
      }   
  }   
    
  sleep(1) ;  
        
    
  //free   
  memcached_free(memc);   
  return 0;   
}  
