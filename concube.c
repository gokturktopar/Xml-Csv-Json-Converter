#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <json/json.h>
#include <ctype.h>
#define BUFFER_SIZE 100
typedef int bool;
#define true 1
#define false 0
#define BOLDBLACK "\033[1m\033[30m"
#define reset "\033[0m"
static void xmltocsv();
static void xml_to_csv(xmlNode * a_node,xmlNode *root_element);
static char input_file_type[5]; // input dosya tipi
static char output_file_type[5]; // output dosya  tipi
static bool inputs_exist_true;// girdilerde true komutu var mı
static int inputs_controll(int argc,char **argv);
static char input_file_name[25];
static char output_file_name[25];
void trim(char *input);
void json_parse_for_xml(json_object *jobj, xmlNodePtr node);
void jsonparse_json_to_xml();
static xmlNodePtr rootNode;
static json_object *tmp=NULL;
static json_object *objectParsed=NULL;
int flag=0;
#define N 20
#define LENGTH 10
static void fileRead(char *filename);
static void parseCsv(char *str, char *arrToken[]);
static void writeCsv(char *str,FILE *fileName);
static void csvToXml();
static void csvToCompXml();
static void xmlTakeRoot();
static void xmlToJson(xmlNode * a_node,xmlNode *root_element);
static void csvToJson();
char *words[256];
char *elements[100];
int i=0;
int sizeOfElements=0;
int numberOfRow=0;  //count of rows
int numbContentRow[250]; // tokens in the rows
int rowNumb=0;
json_object *jobj1;
json_object *jobj2;
json_object *jarray;
void json_to_csv();
void json_parse(json_object * jobj);

int main(int argc,char **argv){

		int programstart=inputs_controll(argc,argv);
		if(programstart==1){

			if(strcmp(input_file_type,"xml")==0&&strcmp(output_file_type,"csv")==0){
			printf("source: %s \n",input_file_name);
				xmltocsv();

			}
			else if(strcmp(input_file_type,"xml")==0&&strcmp(output_file_type,"json")==0){
				xmlTakeRoot();
			}
			else if(strcmp(input_file_type,"csv")==0&&strcmp(output_file_type,"xml")==0){
				fileRead(input_file_name);
				if(inputs_exist_true){

				csvToCompXml();
				}
				else{
				csvToXml();
				}
			}
			else if(strcmp(input_file_type,"csv")==0&&strcmp(output_file_type,"json")==0){
				fileRead(input_file_name);
				csvToJson();
			}
			else if(strcmp(input_file_type,"json")==0&&strcmp(output_file_type,"csv")==0){

				json_to_csv();
			}

			else if(strcmp(input_file_type,"json")==0&&strcmp(output_file_type,"xml")==0){
				jsonparse_json_to_xml();
			}
		}

	return 0;
	}
static int inputs_controll(int argc,char **argv){ // komut satırı girdileri kontrol
	int correctall=1;
	if(strcmp(argv[1],"man")==0||argc<=1){
	printf(BOLDBLACK"				User Commands \n");
	printf(BOLDBLACK"	NAME\n \n");
      	printf(reset"	concube \n \n");

printf(BOLDBLACK"	SYNOPSIS\n \n");
 printf(reset"      ./concube.run -i abc.xml -o abc.csv \n \n");

printf(BOLDBLACK"	DESCRIPTION\n\n");
printf(reset"     	This program  convert xml,json and csv files each others.\n\n");

printf(BOLDBLACK"      -i\n\n");
printf(reset"	It state input file in current directory. [ -i abc.xml ]\n\n");

printf(BOLDBLACK"	-o\n\n");
printf(reset"	It state output file that you want to create. File creates in current directory [ -o abc.csv ]\n\n");

printf(BOLDBLACK"	-t\n\n");
printf(reset"	It state output file type  that you want to create.You can use instead of -o ...  [ -t csv/xml/json ]\n\n");

printf(BOLDBLACK"	-true \n\n");
printf(reset"	When output file type is xml , it provide that xml tags are created as attibutes.");
	correctall=0;
}

	else{
	int ab=0;
	bool command_t=false;
	bool command_o=false;
	char search[3]=".";
	for(ab=0;ab<argc;ab++){
		if(strcmp(argv[ab],"-i")==0&&ab+1<argc){
				FILE *file;
			if (file=fopen(argv[ab+1],"r"))
			{
				strcpy(input_file_name,argv[ab+1]);
				fclose(file);
				char *token;

				token=strtok(argv[ab+1],search);
				token=strtok(NULL,search);
				if (strcmp(token,"xml")==0)
				{
					strcpy(input_file_type,"xml");
				}
				else if (strcmp(token,"json")==0)
				{
					strcpy(input_file_type,"json");
				}
				else if (strcmp(token,"csv")==0)
				{
					strcpy(input_file_type,"csv");
				}
				else
				{
					printf(BOLDBLACK"Your input file's extension is invalid...\n'");
				correctall=0;
				}
			}
			else
			{
					printf(BOLDBLACK"There is no file such a your input file...\n");
			correctall=0;
			}
		}
		if(strcmp(argv[ab],"-o")==0&&ab+1<argc){
			char *token2;
			strcpy(output_file_name,argv[ab+1]);
			token2=strtok(argv[ab+1],search);
			token2=strtok(NULL,search);
			if (strcmp(token2,"xml")==0)
			{
				strcpy(output_file_type,"xml");
				command_o=true;
			}
			else if (strcmp(token2,"json")==0)
			{
				strcpy(output_file_type,"json");
				command_o=true;
			}
			else if (strcmp(token2,"csv")==0)
			{
				strcpy(output_file_type,"csv");
				command_o=true;
			}
			else
			{
				printf(BOLDBLACK"Your output file's extension is invalid...\n'");
				correctall=0;
			}
		}
		if(strcmp(argv[ab],"-t")==0&&ab+1<argc){
			if (strcmp(argv[ab+1],"xml")==0)
			{
				strcpy(output_file_type,"xml");
				command_t=true;
			}
			else if (strcmp(argv[ab+1],"json")==0)
			{
				command_t=true;
				strcpy(output_file_type,"json");

			}
			else if (strcmp(argv[ab+1],"csv")==0)
			{
				strcpy(output_file_type,"csv");
				command_t=true;
			}
			else
			{
				printf(BOLDBLACK"Error ...-t contains xml/csv or json...\n'");
				correctall=0;
			}
		}

	}
	// -true kontrolü
	for(ab=0;ab<argc;ab++){
		if(strcmp(argv[ab],"-true")==0){
			if(strcmp(output_file_type,"xml")==0){
				inputs_exist_true=true;
			}
			else{
				printf(BOLDBLACK"Syntax error...'true' command arguments can use just when output file type is xml...\n");
				correctall=0;
			}
		}
	}
	// -t verildiyse output dosyasına isim atama
	if(command_t&&!command_o){
	if (strcmp(output_file_type,"xml")==0)
			{
				strcpy(output_file_name,"result.xml");

			}
			else if (strcmp(output_file_type,"json")==0)
			{
				strcpy(output_file_name,"result.json");
			}
			else if (strcmp(output_file_type,"csv")==0)
			{
				strcpy(output_file_name,"result.csv");
			}
		}
	}
	printf(reset"input file name :%s \n ",input_file_name);
printf(reset"input file type :%s \n ",input_file_type);
printf(reset"output file name :%s \n ",output_file_name);
printf(reset"output file type :%s \n ",output_file_type);

	return correctall;
	}

	static void xmltocsv()	{
		xmlDoc *doc = NULL;
		xmlNode *root_element = NULL;
		const char *Filename = input_file_name;

		doc = xmlReadFile(Filename, NULL, 0);

		if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
		else
	{
		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);
		//



		xml_to_csv(root_element,root_element);

		/*free the document*/
		xmlFreeDoc(doc);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();
	}
	static void xml_to_csv(xmlNode * a_node,xmlNode *root_element)
	{
		 char* tags[100];// tag isimleri
		char* contents[100];// tagların içeriği
		int counter2;
		int counter;
 		FILE *fcsv;
		fcsv=fopen(output_file_name,"w");
		counter2=0;
		counter=0;
		int c=0;
		int same=0;
	 void x(xmlNode * a_node){
		c++;
		xmlNode *cur_node = NULL;
		for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

			if (cur_node->type == XML_ELEMENT_NODE&&cur_node!=root_element) {
					if(cur_node->parent==root_element){
					trim(cur_node->children->content);


					if(strcmp(cur_node->children->name,"")==0){

					if(strcmp(cur_node->children->content,"")!=0){

					tags[counter2]=(char*) malloc(150);

					strcpy(tags[counter2],cur_node->name);
					fprintf(fcsv,"%s,",tags[counter2]);
					counter2++;

					}

					}
					if(strcmp(cur_node->children->content,"")!=0){
					//printf("deneme: %s",cur_node->name);
					tags[counter2]=(char*) malloc(150);
					strcpy(tags[counter2],cur_node->name);
					fprintf(fcsv,"%s,",tags[counter2]);
					counter2++;

					}

					}
					else{
					trim(cur_node->children->content);
					if(cur_node->parent->parent==root_element){
						if(strcmp(cur_node->children->content,"")!=0){
					int k;
					bool foundsame=false;
					for(k=0;k<counter2;k++){
						if(strcmp(cur_node->name,tags[k])==0){
						foundsame=true;
						}
					}
					if(foundsame==false){
						tags[counter2]=(char*) malloc(150);
						strcpy(tags[counter2],cur_node->name);
						printf("tagco  %s \n",cur_node->children->content);
						fprintf(fcsv,"%s,",tags[counter2]);
						counter2++;
					}



					}
			}
			else{
					char src[50], dest[50],sepa[3],temp[50];
					strcpy(src,cur_node->name);
					strcpy(sepa,"->");
				   strcpy(temp, cur_node->parent->name);
					//printf("ilk: %s \n",temp);
					strcat(temp,sepa);

					strcat(temp,src);
					strcpy(dest,temp);

					//printf("tagco2 %s \n",tags[counter2]);
					strcpy(temp,"");

					//printf("sil: %s \n",temp);
					xmlNode *temp_node=NULL;
						temp_node=cur_node;
					temp_node=temp_node->parent->parent;
					//printf("girmeden %s \n",temp_node->name);
					while(temp_node->parent!=root_element){
						strcpy(temp,"");
					  	 strcpy(temp, temp_node->name);
						 strcat(temp,sepa);
						strcat(temp,dest);
						strcpy(dest,temp);

					   	temp_node=temp_node->parent;
						}

					int k;
					int foundsame=0;

					k=0;

					for(k=0;k<counter2;k++){
						printf("array %d:%s \n",k,tags[k]);
						if(strcmp(tags[k],dest)==0){
						foundsame=1;

						//printf("%d . elemena %s \n",k,tags[k]);
						//printf("tags%d : %s dest %s \n",k,tags[k],dest);
						}
					}

					if(foundsame==0){
					printf("%d \n",counter2);
					tags[counter2]=(char*) malloc(strlen(dest));
					strcpy(tags[counter2],dest);
					//printf("dest %s \n",dest);
					fprintf(fcsv,"%s,",tags[counter2]);

					//printf("array :%s \n",tags[counter2]);
					counter2++;
					}

						}

				}




				if(cur_node->children!=NULL){
						trim(cur_node->children->content);

					if(strcmp(cur_node->children->content,"")!=0){
				contents[counter]=(char*) malloc(150);
				contents[counter]=cur_node->children->content;
				counter++;


				}
				}

				else{
				contents[counter]=(char*) malloc(150);
				contents[counter]=" ";
				counter++;
			}



			}

			x(cur_node->children);
		}
		}
		x(a_node);


		int i;
		//printf("sametags%d : c%d, \n",sametags,counter2);
		fprintf(fcsv,"\n");
		int co=0;
		//printf(" counter2:%d \n",counter2);
		for(i=0;i<counter;i++){
			co++;
			//printf(" co:%d \n",co);
			if(co==counter2){

			fprintf(fcsv,"%s \n",contents[i]);
			co=0;
			}
			else{
			fprintf(fcsv,"%s,",contents[i]);
			}


		}


		fclose(fcsv);

	}

	void trim(char *input)
		{
		   char *dst = input, *src = input;
		   char *end;

		   // Skip whitespace at front...
		   //
		   while (isspace((unsigned char)*src))
		   {
		      ++src;
		   }

		   // Trim at end...
		   //
		   end = src + strlen(src) - 1;
		   while (end > src && isspace((unsigned char)*end))
		   {
		      *end-- = 0;
		   }

		   // Move if needed.
		   //
		   if (src != dst)
		   {
		      while ((*dst++ = *src++));
		   }
		}
		/* CSV TO XML */
		static void fileRead(char *filename){ // csv to xml
			FILE *file = fopen(filename,"read");
			if(file == NULL){
			printf("file read error\n");
		}else {
				printf("file read success\n");

		}

		int c=0;
		char str[BUFFER_SIZE];

		fgets(str, BUFFER_SIZE, file);
		parseCsv(str, elements);
		sizeOfElements=i;
		i=0;

		while(fgets(str, BUFFER_SIZE, file)){
			//printf("char: %s\n",str);
			parseCsv(str,words);

		}
		//numberOfRow=i/sizeOfElements;
		fclose(file);


	}
	static void parseCsv(char *str, char *arrToken[]){

		numbContentRow[rowNumb]=0;
		char *token;
		const char delim[2]=",\n";
		token=strtok(str,delim);


		while(token!=NULL){
			printf("%s ", token);

			numbContentRow[rowNumb]++;

			arrToken[i]=malloc(strlen(token));
			strcpy(arrToken[i],token);

			token=strtok(NULL,delim);

			i++;

		}
		rowNumb++;



	}
	static void writeCsv(char *str,FILE *fileName){

		if(fileName!=NULL){
			if(fputs(str,fileName)){
				printf("writing error");
			}
		}



	}
	static void csvToCompXml(){
		xmlDocPtr doc = NULL;	/* document pointer */
		xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
		doc = xmlNewDoc(BAD_CAST output_file_name);
		root_node = xmlNewNode(NULL, BAD_CAST "root");
		xmlDocSetRootElement(doc, root_node);

		int i=0;
		int j=0;

		int temp=0;
		for(i=1;i<rowNumb;i++){
		node = xmlNewChild(root_node, NULL, BAD_CAST "class",NULL);
		for(j=0;j<numbContentRow[i];j++){


			xmlNewProp(node,BAD_CAST elements[j],BAD_CAST words[j+temp]);//creates elements with content
		}
		temp=numbContentRow[i]+temp;
	}
		xmlSaveFormatFileEnc(output_file_name, doc, "UTF-8", 1);
		/*free the document */
		xmlFreeDoc(doc);
		xmlMemoryDump();
	}
	static void csvToXml(){
		/*FILE *fileptr;
		fileptr= fopen("names.xml", "rb+");
		if(fileptr==NULL){
			fileptr=fopen("names.xml","wb");
		}*/

		xmlDocPtr doc = NULL;	/* document pointer */
		xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
		xmlNodePtr parent_node=NULL;
		doc = xmlNewDoc(BAD_CAST "names.xml");
		root_node = xmlNewNode(NULL, BAD_CAST "root");
		xmlDocSetRootElement(doc, root_node);

		/* xmlNewChild() creates a new node, which is "attached" as child node of root_node node. */
		int i=0;
		int j=0;
		int countForParents=0;
		char *buffer=malloc(30);
		char parent[50]="elements";

		int temp=0;
		for(i=1;i<rowNumb;i++){
			parent_node = xmlNewChild(root_node, NULL,BAD_CAST parent,NULL );//gruplari elements tag'i altinda toplamak icin

			for(j=0;j<numbContentRow[i];j++){


				xmlNewChild(parent_node, NULL, BAD_CAST elements[j],BAD_CAST words[j+temp]);//creates elements with content
			}
			temp=numbContentRow[i]+temp;
		}
		/* Dumping document to stdio or file */
		//xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, "UTF-8", 1);
		xmlSaveFormatFileEnc(output_file_name, doc, "UTF-8", 1);
		//xmlSaveFile("xmlfile_copy.xml", doc);
		/*free the document */
		xmlFreeDoc(doc);
		xmlMemoryDump();

	}
	//csv to xml son
	void json_to_csv(){
		FILE *file=fopen(input_file_name,"r");
		char destination[500];
		strcpy(destination,"");
		char buf[BUFFER_SIZE];
		if(file==NULL){
			printf("File is not valid");
		}
		else{
			int linecounter=0;// 2.satırdan başlayıp rootu almamak için


			char* elements[500];
			int counter=0;
			while(fgets(buf,BUFFER_SIZE,file)){
			trim(buf);
			strcat(destination,buf);

			}

		}
		fclose(file);
		json_object * jobj = json_tokener_parse(destination);
		json_parse(jobj);
	}
	void json_parse(json_object * jobj) {
	char* tags[100];
		const char* content[100];
		int tagcounter=0;
		int contentcounter=0;
	int i;
	int sametags=0;
	FILE *fcsv=fopen(output_file_name,"w");
	int firstlinelenght=0;
	int contents[50][50];

	void x(json_object *jobj){
	enum json_type type;
	json_object *tmp=NULL;

	json_object_object_foreach(jobj, key, val) {

		type = json_object_get_type(val);
		switch (type) {

			case json_type_string:

				content[contentcounter]=(char*)malloc(strlen(json_object_get_string(val)));
				int k;
				int controlexist=0;
				for(k=0;k<tagcounter;k++){
					if(strcmp(tags[k],key)==0){
					controlexist=1;
						}
				}
				if(controlexist==0){
					tags[tagcounter]=(char*)malloc(strlen(key));

					tags[tagcounter]=key;
					tagcounter++;
				}
				else{
					sametags++;
				}

				content[contentcounter]=json_object_get_string(val);

				contentcounter++;


			break;
			case json_type_int:

			content[contentcounter]=(char*)malloc(strlen(json_object_get_string(val)));
				int k1;
				int controlexist1=0;
				for(k1=0;k1<tagcounter;k1++){
					if(strcmp(tags[k1],key)==0){
					controlexist1=1;
						}
				}
				if(controlexist1==0){
					tags[tagcounter]=(char*)malloc(strlen(key));
					tags[tagcounter]=key;
					tagcounter++;
				}
				else{
					sametags++;
				}
				content[contentcounter]=json_object_get_string(val);
				contentcounter++;
			break;

		case json_type_double:

			content[contentcounter]=(char*)malloc(strlen(json_object_get_string(val)));
				int k2;
				int controlexist2=0;
				for(k2=0;k<tagcounter;k2++){
					if(strcmp(tags[k2],key)==0){
					controlexist2=1;
						}
				}
				if(controlexist2==0){
					tags[tagcounter]=(char*)malloc(strlen(key));
					tags[tagcounter]=key;
					tagcounter++;
				}
				else{
					sametags++;
				}
				content[contentcounter]=json_object_get_string(val);
				contentcounter++;
			break;

		case json_type_object:

		//	json_object_object_get_ex(jobj, key,&val);
		//		printf("key: %s :", key);
		//	printf(" value: %s\n", json_object_get_string(val));
			 tmp = json_object_object_get(jobj, key);
                          // json_parse(jobj);

			x(tmp);
			break;

		case json_type_array:
			for (i = 0; i < json_object_array_length(val); i++) {

					tmp = json_object_array_get_idx(val, i);
					x(tmp);


				}

			break;
		}

	}
}
	x(jobj);

	// csv dosyasına yazdırma //
	int k1;
	for(k1=0;k1<tagcounter;k1++){
		if(k1==tagcounter-1){
			fprintf(fcsv,"%s \n",tags[k1]);
		}
		else{
			fprintf(fcsv,"%s,",tags[k1]);
		}
	}

		int co=0;
		int s=0;
		for(s=0;s<contentcounter;s++){
			co++;

			if(co==tagcounter){

			fprintf(fcsv,"%s \n",content[s]);
			co=0;
			}
			else{
			fprintf(fcsv,"%s,",content[s]);
			}


		}
}
	void jsonparse_json_to_xml(){

	FILE *file=fopen(input_file_name,"r");
		char * temp[BUFFER_SIZE];

			char destination[500];
			strcpy(destination,"");
		if(file==NULL){
			printf("File is not valid");
		}
		else{
			int linecounter=0;// 2.satırdan başlayıp rootu almamak için
			char buf[BUFFER_SIZE];

			char* elements[500];
			int counter=0;
			while(fgets(buf,BUFFER_SIZE,file)){

			trim(buf);


				strcat(destination,buf);


				}

			}


		fclose(file);
	json_object *jobj=json_tokener_parse(destination);
	printf("token ettim\n");
	//printf("token ettim\n");
	xmlDocPtr doc = NULL;
	doc =xmlNewDoc(BAD_CAST "convert.xml");


	rootNode=xmlNewNode(NULL,BAD_CAST "root");

	xmlDocSetRootElement(doc, rootNode);
	json_parse_for_xml(jobj,rootNode);
	xmlSaveFormatFileEnc( output_file_name, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlMemoryDump();

}
	void json_parse_for_xml(json_object *jobj,xmlNodePtr node)
	{
	int i=0;
	char *tempKey=NULL;
	xmlNodePtr arrayNode=NULL;
	enum json_type type;

	json_object_object_foreach(jobj,key,val){
		type=json_object_get_type(val);



	switch(type){

			case json_type_string:
					if(inputs_exist_true==0)
						xmlNewChild(node,NULL, BAD_CAST key,BAD_CAST json_object_get_string(val));
					else if(inputs_exist_true==1)
						xmlNewProp(node,BAD_CAST key,BAD_CAST json_object_get_string(val));
			break;
			case json_type_int:
					if(inputs_exist_true==0)
						xmlNewChild(node,NULL, BAD_CAST key,BAD_CAST json_object_get_string(val));
					else if(inputs_exist_true==1)
						xmlNewProp(node, BAD_CAST key,BAD_CAST json_object_get_string(val));
			break;
			case json_type_boolean:
					if(inputs_exist_true==0)
						xmlNewChild(node,NULL, BAD_CAST key,BAD_CAST json_object_get_string(val));
					else if(inputs_exist_true==1)
						xmlNewProp(node,BAD_CAST key,BAD_CAST json_object_get_string(val));
			break;
			case json_type_double:
					if(inputs_exist_true==0)
						xmlNewChild(node,NULL, BAD_CAST key,BAD_CAST json_object_get_string(val));
					else if(inputs_exist_true==1)
						xmlNewProp(node,BAD_CAST key,BAD_CAST json_object_get_string(val));
			break;

			case json_type_array:

				tempKey=key;
				for (i = 0; i < json_object_array_length(val); i++) {
					arrayNode=xmlNewChild(node,NULL, BAD_CAST tempKey, NULL);
					tmp = json_object_array_get_idx(val, i);
					objectParsed=json_tokener_parse(json_object_get_string(tmp));
					json_parse_for_xml(objectParsed,arrayNode);

				}

			break;
		}

	}

}
static void xmlTakeRoot()
	{
		xmlDoc *doc = NULL;
		xmlNode *root_element = NULL;
		const char *Filename = input_file_name;
		doc = xmlReadFile(Filename, NULL, 0);

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", Filename);
	}
	else
	{
		/* Get the root element node*/
		root_element = xmlDocGetRootElement(doc);
		//



		xmlToJson(root_element,root_element);

		/*free the document*/
		xmlFreeDoc(doc);
	}
	/*Free the global variables that may have been allocated by the parser.*/
	xmlCleanupParser();
	}



  static void xmlToJson(xmlNode * a_node,xmlNode *root_element)
	{

		const char* tags[100];// tag isimleri
		char* contents[100];// taglarÄ±n iÃ§eriÄŸi
		int counter2;
		int counter;
 		FILE *fcsv;
		fcsv=fopen("merve.csv","w");
		counter2=0;
		counter=0;
		int c=0;
		char parentName[100],star[100];



		void y(xmlNode * a_node){
/*		json_object *jobj;*/
/*		json_object *jstring1;*/
			c++;
			xmlNode *cur_node = NULL;
			for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

/*jobj=json_object_new_object();*/

			if (cur_node->type == XML_ELEMENT_NODE&&cur_node!=root_element) {
					if(cur_node->parent==root_element){
					trim(cur_node->children->content);



					if(strcmp(cur_node->children->name,"")==0){

					if(strcmp(cur_node->children->content,"")!=0){

					tags[counter2]=(char*) malloc(150);
					tags[counter2]=cur_node->name;
					printf("curnode%s\n",tags[counter2]);
/*jstring1 = json_object_new_string(cur_node->content);*/
/*					json_object_object_add(jobj,tags[counter2],jstring1);*/

					counter2++;
					//object
					}

					}
					if(strcmp(cur_node->children->content,"")!=0){


					tags[counter2]=(char*) malloc(150);
					tags[counter2]=cur_node->name;
	//char *src=malloc(strlen("*"));
char src[10];
strcpy(star,cur_node->name);
strcpy(parentName,cur_node->name);

	strcpy(src,"*");
strcat(star,src);
tags[counter2]=star;
	printf("curnode1%s\n",tags[counter2]);

/*strcat(tags[counter2],src);sfdssdfsdfsdfsdfsdfsdfsfd*/
/*					printf("%s",tags[counter2]);*/
/*jstring1 = json_object_new_string(cur_node->content);*/
/*					json_object_object_add(jobj, tags[counter2], jstring1);*/
					counter2++;
					//object
					}

					}
					else{

					if(cur_node->parent->parent==root_element){



						tags[counter2]=(char*) malloc(150);
						tags[counter2]=cur_node->name;
printf("curnode2%s\n",tags[counter2]);
						//printf("tagco %d %s \n",counter2,tags[counter2]);
/*						printf("%s,",tags[counter2]);*/
/*jstring1 = json_object_new_string(cur_node->content);*/
/*					json_object_object_add(jobj, tags[counter2], jstring1);*/
						counter2++;
//object





			}
			else{
					char src[50], dest[50],sepa[3],temp[50];
					strcpy(src,cur_node->name);
					strcpy(sepa,"->");
				   strcpy(temp, cur_node->parent->name);
					//printf("ilk: %s \n",temp);
					strcat(temp,sepa);

					strcat(temp,src);
					strcpy(dest,temp);

					//printf("tagco2 %s \n",tags[counter2]);
					strcpy(temp,"");

					//printf("sil: %s \n",temp);
					xmlNode *temp_node=NULL;
						temp_node=cur_node;
					temp_node=temp_node->parent->parent;
					//printf("girmeden %s \n",temp_node->name);
					while(temp_node->parent!=root_element){
						strcpy(temp,"");
					  	 strcpy(temp, temp_node->name);
						 strcat(temp,sepa);
						strcat(temp,dest);
						strcpy(dest,temp);

					   	temp_node=temp_node->parent;
						}

					int k;
					bool foundsame=false;

					k=0;
/*						printf("dest %s \n",dest);*/
					for(k=0;k<counter2-1;k++){
/*							printf("array :%s \n",tags[k]);*/
/*						if(strcmp(tags[k],dest)==0){*/
/*						foundsame=false;//true*/
/*						//printf("%s \n",tags[k]);*/
/*						}*/
					tags[counter2]=(char*) malloc(150);
					tags[counter2]=cur_node->name;
printf("curnode2%s\n,",tags[counter2]);

/*					printf("%s,",tags[counter2]);*/
					counter2++;//object

					}




						}

				}




				if(cur_node->children!=NULL){
						trim(cur_node->children->content);

					if(strcmp(cur_node->children->content,"")!=0){
				contents[counter]=(char*) malloc(150);
				contents[counter]=cur_node->children->content;
				counter++;


				}
				}

				else{
				contents[counter]=(char*) malloc(150);
				contents[counter]=" ";
				counter++;
			}



			}


			y(cur_node->children);
		}

/*printf (" %s\n",json_object_to_json_string(jobj));*/
FILE *fjson;
fjson=fopen(output_file_name,"w");
json_object *jstring1;
jobj2=json_object_new_object();

jarray = json_object_new_array();
char cutTheStar[100];

for(int z=0;z<counter2;z++){

/*printf("tagz:%s\n",tags[z]);*/
/*if(strcmp(p,"\n")==0){*/
/*if(p!=NULL){*/
/*printf("asdas : %s\n",contents[z]);*/

if(strstr(tags[z],"*")!=NULL){

//if(strcmp(tags[z],"employee")==0){
/*char delimeter[]="*";;
strcpy(cutTheStar,"");

char temp[100];
strcpy(temp,tags[z]);
temp[strlen(temp)-1]=0;
strcpy(cutTheStar,temp);
printf("cutTheStar%s\n",cutTheStar);*/
//cutTheStar=strtok(tags[z],delimeter);
json_object *jobj = json_object_new_object();
for(int a=z+1;a<counter2;a++){

/*for(int t=0;t<counter;t++){*/
jstring1=NULL;
////////////////if(strcmp(tags[a],"root")!=0){
/*if((strcmp(tags[z],"employee")==0)||(strcmp(contents[z],"")==0)){*/
/*jobj = json_object_new_object();*/
/*}*/

/*char *c;*/
/*c=strstr(contents[a],"\n");*/
/*if(c==NULL)*/
if(strstr(tags[a],"*")==NULL){
//if(strcmp(tags[a],"employee")!=0){
/*if(strcmp(contents[z],"    ")!=0){///ta dogru deil*/



/*printf("content%s\n",contents[t]);*/
/*jarray = json_object_new_array();*/
/*json_object_array_add(jarray, jobj);*/
/*json_object_object_add(jobj2, "employee", jarray);*/
jstring1 = json_object_new_string(contents[a]);
json_object_object_add(jobj,tags[a], jstring1);






}
else{
break;}


///}

							}
					json_object_array_add(jarray, jobj);
printf (" JARRAY%s\n",json_object_to_json_string(jarray));

					}

}

json_object_object_add(jobj2, parentName, jarray);
/*printf (" jobj2%s\n",json_object_to_json_string(jobj2));*/
printf (" jobj%s\n",json_object_to_json_string(jobj2));
fprintf(fjson,"%s",json_object_to_json_string(jobj2));
fclose(fjson);
for(int w=0;w<counter;w++)
{
printf("contents:%s\n",contents[w]);
}
for(int n=0;n<counter2;n++)
{


printf("tags:%s\n",tags[n]);
}
		}


		y(a_node);


		int i;
		//printf("sametags%d : c%d, \n",sametags,counter2);
/*		printf("\n");//*/
		int co=0;
		//printf(" counter2:%d \n",counter2);
		for(i=0;i<counter;i++){
			co++;
			//printf(" co:%d \n",co);
			if(co==counter2){

/*			printf("%s \n",contents[i]);//*/
			co=0;
			}
			else{
/*			printf("%s,",contents[i]);//*/
			}


		}


		fclose(fcsv);

	}
	static void csvToJson(){
	FILE *fjson;
	fjson=fopen(output_file_name,"w");
	int i=0;
	int j=0;
	/*json_object *jobj=NULL;*/
	json_object *jobj;
	json_object *jobj2=json_object_new_object();
	/*json_object *jstring=NULL;*/
	json_object *jstring;

		int temp=0;
		json_object *jarray=json_object_new_array();
		for(i=1;i<rowNumb;i++){
	/*	printf ("numberOfRow:%d\n",rowNumb);*/
		jobj=json_object_new_object();
	/*	jstring=NULL;*/
	/*		printf ("numberOfRow:%d\n",numbContentRow[i]);*/

			for(j=0;j<numbContentRow[i];j++){
			jstring = json_object_new_string(words[j+temp]);
	/*		printf (" %s\n",json_object_to_json_string(jstring));*/
			json_object_object_add(jobj, elements[j], jstring);
	/*printf ("i:%d\n",i);*/
	/*printf ("j:%d\n",j);*/



		}
			json_object_array_add(jarray,jobj);
			temp=numbContentRow[i]+temp;
		}
	json_object_object_add(jobj2,"Employee",jarray);
	printf (" %s\n",json_object_to_json_string(jobj2));
	fprintf(fjson,"%s",json_object_to_json_string(jobj2));
	fclose(fjson);


	}
