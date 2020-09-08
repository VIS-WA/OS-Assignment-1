#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

int main(int argc , char *argv[])
{
	char buf[1000];
	struct stat stat_n,stat_o,stat_d;
	int error1,error2,error3,r1,r2;
	error1 = stat(argv[1],&stat_n);
	error2 = stat(argv[2],&stat_o);
	error3 = stat(argv[3],&stat_d);
	if(error3==0)
	{
		sprintf(buf,"Directory is created: Yes\n\n");
		write(1,buf,strlen(buf));
	}
	else
	{
		sprintf(buf,"Directory is created: No\n\n");
		write(1,buf,strlen(buf));
	}
	/*if(errno==2)
	  printf("Directory is created: No\n\n");
	  else
	  printf("Directory is created: Yes\n\n");*/
	if(error1!=0)
		perror("Error regarding newfile");
	if(error2!=0)
		perror("Error regarding oldfile");
	if(error3!=0)
		perror("Error regarding Directory");

	mode_t n_t=stat_n.st_mode , o_t=stat_o.st_mode, d_t = stat_d.st_mode;

	if(error1==0 && error2==0)
	{

		long long int i,j,sz=-100000,p1,p2;
		char *flag="Yes";
		int fd_new,fd_old;
		char *buf_new,*buf_old,*buf_new2,*buf_old2;
		fd_new=open(argv[1],O_RDONLY);
		buf_new=(char *)malloc(-sz*sizeof(char));
		p1=lseek(fd_new,0,SEEK_SET);
		fd_old=open(argv[2],O_RDONLY);
		buf_old=(char *)malloc(-sz*sizeof(char));
		p2=lseek(fd_old,0,SEEK_END);
		buf_new2=(char *)malloc(-sz*sizeof(char));
		buf_old2=(char *)malloc(-sz*sizeof(char));

		int end1,end2;
		end1=lseek(fd_new,0,SEEK_END);
		end2=lseek(fd_old,0,SEEK_END);
		lseek(fd_new,0,SEEK_SET);
		if(end1!=end2)
		{
			flag="No";
		}
		else
		{
			while((lseek(fd_old,0,SEEK_CUR)+sz)>=0)
			{
				lseek(fd_old,sz,SEEK_CUR);
				r1=read(fd_old,buf_old,-sz);
				lseek(fd_old,sz,SEEK_CUR);
				lseek(fd_new,0,SEEK_CUR);
				r2=read(fd_new,buf_new,-sz);

				for(i=0,j=-sz-1 ;i<j; i++)
				{
					if(buf_old[i]!=buf_new[j])
					{
						flag="No";
						break;
					}
					j--;
				}
			}
			read(fd_new,buf_new2,-sz);
			p1=lseek(fd_old,0,SEEK_CUR);
			lseek(fd_old,0,SEEK_SET);
			read(fd_old,buf_old2,p1);

			for(i=0,j=strlen(buf_old2)-1;i<j;i++,j--)
			{
				if(buf_old2[i]!=buf_new2[j])
				{
					flag="No";
					break;
				}
			}
		}
		close(fd_old);
                close(fd_new);

		sprintf(buf, "\nWhether file contents are reversed in newfile:%s\n\n", flag);
		write(1, buf, strlen(buf));
	}
	else
	{
		sprintf(buf, "\nWhether file contents are reversed in newfile: No\n\n");
		write(1, buf, strlen(buf));
	}

	int i;

	for(i=1;i<=3;i++)
	{
		if((i==1 && error1==0) || (i==2 && error2==0) || (i==3 && error3==0))
		{
			mode_t fas;
			char *f_name;
			if(i==1)
			{
				fas=n_t;
				f_name="newfile";
			}
			if(i==2)
			{
				fas=o_t;
				f_name="oldfile";
			}
			if(i==3)
			{
				fas=d_t;
				f_name="directory";
			}

			sprintf(buf,"User has read permissions on %s:     %s\n",f_name, fas&S_IRUSR ? "Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"User has write permission on %s:     %s\n",f_name, fas&S_IWUSR ?"Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"User has execute permission on %s:   %s\n",f_name,fas&S_IXUSR ?"Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Group has read permissions on %s:    %s\n",f_name,fas&S_IRGRP ? "Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Group has write permission on %s:    %s\n", f_name,fas&S_IWGRP ?"Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Group has execute permission on %s:  %s\n",f_name,fas&S_IXGRP ? "Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Others has read permissions on %s:   %s\n", f_name,fas&S_IROTH ?"Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Others has write permission on %s:   %s\n", f_name,fas&S_IWOTH ?"Yes" : "No");
			write(1,buf,strlen(buf));
			sprintf(buf,"Others has execute permission on %s: %s\n\n",f_name,fas&S_IXOTH ?"Yes" : "No");
			write(1,buf,strlen(buf));
		}

	}
	return 0;
}
