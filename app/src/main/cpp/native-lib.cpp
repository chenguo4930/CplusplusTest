#include <jni.h>
#include <string>
#include <stdlib.h>
#include "../jni/log_utils.h"


extern "C"
jstring
Java_com_example_cheng_cplusplustest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //将C的字符串转为一个java字符串
    return env->NewStringUTF(hello.c_str());
}

extern "C"
jstring Java_com_example_cheng_cplusplustest_MainActivity_getString2Fromc
        (JNIEnv *env, jobject, jint num) {
    return env->NewStringUTF("C Sting2");
}

//jobject和jclass 代表native方法所属类的class对象，访问class的属性和方法都是通过它去获取
//1)当native方法为静态方法时：
//jclass 代表native方法所属类的class对象（MainActivity.class）
//2）当native方法为非静态方法时
//jobject 代表native方法所属的对象

//JNI基本数据
//Java基本数据类型与JNI数据类型的映射关系
//Java类型->JNI类型-C类型

/*
 * boolean jboolean
 * byte jbyte
 * char jchar
 * short jshort
 * int jint
 * long jlong
 * float jfloat
 * double jdouble
 * void void
 * */

/*
 * 引用类型
 * String jstring
 * object jobject
 * 数组,基本数据类型的数组
 * byte[] jByteArray
 * 对象数组
 * object[](String[]) jobjectArray
 * */

//1、访问属性
//修改属性值key
extern "C"
jstring Java_com_example_cheng_cplusplustest_MainActivity_accessField
        (JNIEnv *env, jobject jobj) {
    //jobj是T对象，MainActivitu.class
    jclass cls = env->GetObjectClass(jobj);
    //jfieldID
    //属性名称，最后一位为属性签名
    jfieldID fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");
    //guo >> guo gold
    //获取了key属性的值
    jstring jstr = (jstring) env->GetObjectField(jobj, fid);
    char *c_str = (char *) env->GetStringUTFChars(jstr, NULL);
    //拼接新的字符串
    char text[20] = "supper ";
    strcat(text, c_str);
    //c字符串转成jstring
    jstring new_jstr = env->NewStringUTF(text);
    //修改key
    env->SetObjectField(jobj, fid, new_jstr);
    return new_jstr;
}
//2访问静态属性
extern "C"
void Java_com_example_cheng_cplusplustest_MainActivity_accessStaticField(JNIEnv *env,
                                                                         jobject jobj) {

    // jclass
    jclass cls = env->GetObjectClass(jobj);
    //jfield
    jfieldID fid = env->GetStaticFieldID(cls, "count", "I");
    //GetStatic<Type>Field
    jint count = env->GetStaticIntField(cls, fid);
    count++;
    //修改
    env->SetStaticIntField(cls, fid, count);
}
//JNIEXPORT void JNICALL Java_com_example_cheng_cplusplustest_MainActivity_accessStaticField
//        (JNIEnv *env, jobject jobj){
//    jclass cls = env->GetObjectClass(jobj);
//    //jfield
//    jfieldID fid = env->GetStaticFieldID(cls,"count","I");
//    //GetStatic<Type>Field
//    jint count = env->GetStaticIntField(cls,fid);
//    count++;
//    //修改
//    env->SetStaticIntField(cls,fid,count);
//
//}

//3、访问java方法
extern "C"
void Java_com_example_cheng_cplusplustest_MainActivity_accessMethod(JNIEnv *env, jobject jobj) {

    // jclass
    jclass cls = env->GetObjectClass(jobj);
    //jmethodID
    jmethodID mid = env->GetMethodID(cls, "getRandomInt", "(I)I");
    //调用
    //Call<Type>Method  ,属性是Get，方法是Call
    jint random = env->CallIntMethod(jobj, mid, 200);
    LOGW("random = %d", random);
}

//4、访问java静态方法
//借用java api 产生一个UUID字符串，作为文件的名称
extern "C"
void Java_com_example_cheng_cplusplustest_MainActivity_accessStaticMethod(JNIEnv *env,
                                                                          jobject jobj) {
    // jclass
    jclass cls = env->GetObjectClass(jobj);
    //jmethodID
    jmethodID mid = env->GetStaticMethodID(cls, "getUUID", "()Ljava/lang/String;");
    //调用
    jstring uuid = (jstring) env->CallStaticObjectMethod(cls, mid);
    //随机文件名称 uuid.txt
    //jstring -> char*
    //isCopy JNI_false,代表java和c操作的是同一个字符串
    char *uuid_str = (char *) env->GetStringUTFChars(uuid, NULL);
    //拼接
    char filename[100] = {0};
    sprintf(filename, "f://%s.txt", uuid_str);
    LOGW("random = 1");
    FILE *fp = fopen(filename, "w");
    fputs(uuid_str, fp);
    fclose(fp);

}

//访问构造方法
//使用java.util.Date 产生一个当前的时间戳
//签名代表参数列表 ,I = int 简称
//访问构造函数，部分共有还是私有
extern "C"
jobject Java_com_example_cheng_cplusplustest_MainActivity_accessConstructor(JNIEnv *env,
                                                                            jobject instance) {
    jclass cls = env->FindClass("java/util/Date");
    //jmethodId
    jmethodID constructor_mid = env->GetMethodID(cls, "<init>", "()V");
    //实例化一个Date对象
    jobject data_obj = env->NewObject(cls, constructor_mid);
    //调用getTime方法,
    jmethodID mid = env->GetMethodID(cls, "getTime", "()J");
    jlong time = env->CallLongMethod(data_obj, mid);

    LOGW("time=%lld", time);
    return data_obj;
}

extern "C"
void Java_com_example_cheng_cplusplustest_MainActivity_accessNonvirtualMethod(JNIEnv *env,
                                                                              jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    //获取man属性（对象）
    jfieldID fid = env->GetFieldID(cls, "human", "Lcom/example/cheng/cplusplustest/Human;");
    //获取
    jobject human_obj = env->GetObjectField(jobj, fid);

    //执行sayHi方法
    jclass human_cls = env->FindClass("com/example/cheng/cplusplustest/Human"); //注意：传父类的名称
    jmethodID mid = env->GetMethodID(human_cls, "sayHi", "()V");

    //执行
//    env->CallObjectMethod( human_obj, mid);
    //调用的父类的方法
    env->CallNonvirtualObjectMethod(human_obj, human_cls, mid);
}

//中文问题  解决中文乱码问题gb2312编码，传入字符为utf-16
extern "C" jstring Java_com_example_cheng_cplusplustest_MainActivity_chineaseChars
        (JNIEnv *env, jobject jobj, jstring in) {
    //输出
    //char *c_str = (*env)->GetStringUTFChars(env, in, JNI_FALSE);
    //printf("%s\n",c_str);

    //c -> jstring
    char *c_str = "马蓉与宋江";
    //char c_str[] = "马蓉与宋喆";
    //jstring jstr = (*env)->NewStringUTF(env, c_str);
    //执行String(byte bytes[], String charsetName)构造方法需要的条件
    //1.jmethodID
    //2.byte数组
    //3.字符编码jstring

    jclass str_cls = env->FindClass("java/lang/String");
    jmethodID constructor_mid = env->GetMethodID(str_cls, "<init>", "([BLjava/lang/String;)V");

    //jbyte -> char
    //jbyteArray -> char[]
    jbyteArray bytes = env->NewByteArray((jsize) strlen(c_str));
    //byte数组赋值
    //0->strlen(c_str)，从头到尾
    //对等于，从c_str这个字符数组，复制到bytes这个字符数组
    env->SetByteArrayRegion(bytes, 0, (jsize) strlen(c_str), (const jbyte *) c_str);

    //字符编码jstring
    jstring charsetName = env->NewStringUTF("GB2312");

    //调用构造函数，返回编码之后的jstring
    return (jstring) env->NewObject(str_cls, constructor_mid, bytes, charsetName);
}

int compare(int *a, int *b) {
    return (*a) - (*b);
}

//数组
extern "C" void Java_com_example_cheng_cplusplustest_MainActivity_giveArray
        (JNIEnv *env, jobject jobj, jintArray array) {
    //拿到数组指针jintArray -> jint 指针 -> c int 数组
    jint *elems = env->GetIntArrayElements(array, NULL);
    //数组长度
    int len = env->GetArrayLength(array);
    //排序
    qsort(elems, (size_t) len, sizeof(jint), (int (*)(const void *, const void *)) compare);

    //同步
    //mode
    //0, Java数组进行更新，并且释放C/C++数组
    //JNI_ABORT, Java数组不进行更新，但是释放C/C++数组
    //JNI_COMMIT，Java数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）
    env->ReleaseIntArrayElements(array, elems, JNI_COMMIT);

}

//返回数组
extern "C" jintArray Java_com_example_cheng_cplusplustest_MainActivity_getArray
        (JNIEnv *env, jobject jobj, jint len) {

    //创建一个指定大小的数组
    jintArray jint_arr = env->NewIntArray(len);
    jint *elems = env->GetIntArrayElements(jint_arr, NULL);
    int i = 0;
    for (; i < len; ++i) {
        elems[i] = i;
    }

    //同步
    env->ReleaseIntArrayElements(jint_arr, elems, 0);
    return jint_arr;
}

//JNI 引用变量
//引用类型:局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

//局部引用，通过DeleteLocalRef手动释放对象
//1、访问一个很大的java对象，还要进行耗时操作
//2、创建了大量的局部引用占用了太多的内存，而且这些局部引用跟后面的使用没有关联性


//模拟：循环创建数组
extern "C" jintArray Java_com_example_cheng_cplusplustest_MainActivity_localRef
        (JNIEnv *env, jobject jobj) {
    for (int i = 0; i < 5; ++i) {
        //创建Date对象
        jclass cls = env->FindClass("java/util/Date");
        jmethodID constructor_mid = env->GetMethodID(cls, "<init>", "()V");
        jobject obj = env->NewObject(cls, constructor_mid);

        //此处胜率100万行

        //通知垃圾回收器回收这些对象
        env->DeleteLocalRef(obj);

        //此处神雕1万行


    }
}

//全局引用，多个方法里面都可以使用
//共享，手动控制内存使用
jstring global_str;

extern "C" void Java_com_example_cheng_cplusplustest_MainActivity_createGlobalRef
        (JNIEnv *env, jobject jobj) {
    jstring obj = env->NewStringUTF("jni developmen is powerfal");
    global_str = (jstring) env->NewGlobalRef(obj);
}

extern "C" jstring Java_com_example_cheng_cplusplustest_MainActivity_getGlobalRef
        (JNIEnv *env, jobject jobj) {
    return global_str;
}

extern "C" void Java_com_example_cheng_cplusplustest_MainActivity_deleteGlobalRef
        (JNIEnv *env, jobject jobj) {
    env->DeleteGlobalRef(global_str);
}

//弱引用
//节省内存，在内存不足时可以是释放所引用的的对象
//可以引用一个不常用的对象，如果为NULL， 临时创建
//创建：newWeakGlobalRef,销毁：DeleteGlobalWeakRef

//异常处理
//1.保证Java代码可以运行
//2.补救措施保证C代码继续运行

//JNI自己抛出的异常，在Java层无法被捕捉，只能在C层清空
//用户通过ThrowNew抛出的异常，可以在Java层捕捉
extern "C" void Java_com_example_cheng_cplusplustest_MainActivity_exeception
        (JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    jfieldID fid = env->GetFieldID(cls, "key2", "Ljava/lang/String;");
    //检测key2这个属性是否有问题
    //检测是否发生Java异常
    jthrowable exception = env->ExceptionOccurred();
    if (exception != NULL) {
        //发生异常，亲空异常，保证让java代码继续运行
        env->ExceptionClear();

        //补救措施
        fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");
    }
    //获取属性的值
    jstring jstr = (jstring) env->GetObjectField(jobj, fid);
    char *str = (char *) env->GetStringUTFChars(jstr, NULL);

    //对比属性值是否合法
    if (strcmp(str, "super guo sheng") != 0) {
        //以为抛出异常，给Java层处理
        jclass newExcCls = env->FindClass("java/lang/IllegalArgumentException");
        env->ThrowNew(newExcCls, "key's values is invalid！");
    }
}

//缓存策略
extern "C" void
Java_com_example_cheng_cplusplustest_MainActivity_cached(JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    //获取jfieldId值获取一次
    //局部静态变量：只能在这个方法里面使用，但是它从这个方法调用之后，一直存储在内存中
    static jfieldID key_id = NULL;
    if (key_id == NULL) {
        key_id = env->GetFieldID(cls, "key", "Ljava/lang/String;");
        LOGW("获取GetFieldId");
    }

}

//初始化全局变量，动态库加载完成之后，立即缓存起来
jfieldID key_fid;
jmethodID random_mid;
extern "C" void
Java_com_example_cheng_cplusplustest_MainActivity_initIds(JNIEnv *env, jobject jobj) {
    jclass cls = env->GetObjectClass(jobj);
    key_fid = env->GetFieldID(cls, "key", "Ljava/lang/String;");
    random_mid = env->GetMethodID(cls, "getRandomInt", "(I)I");
}










/** C++本地方法实现
 * @author Created by jiangdongguo on 2017-5-24 9:26:10
 * @csdn博客 http://blog.csdn.net/andrexpert
 */
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
//#include <iconv.h>

#define LOG_TAG "laojiang"
// 不带格式日志
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,"%s",__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,"%s",__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,"%s",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s",__VA_ARGS__)
// 带格式日志
#define LOG_I(format,...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,format,__VA_ARGS__)
#define LOG_D(format,...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,format,__VA_ARGS__)
#define LOG_W(format,...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,format,__VA_ARGS__)
#define LOG_E(format,...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,format, __VA_ARGS__)
// 声明函数
char* jstringTostring(JNIEnv* env, jstring jstr);
int compare(const void *a,const void *b);

/**
 *  Java访问本地函数，返回一个字符串
 * */
JNIEXPORT jstring JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getStringFromC
        (JNIEnv *env, jclass jobj){
    return (*env)->NewStringUTF(env,"My Name is jiangdongguo--2017");
}

/**
 *  Java访问本地函数，返回一个拼接字符串
 * */
JNIEXPORT jstring JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getString2FromC
        (JNIEnv *env, jobject jobj, jstring j_str){
    // jstring转char *
    char* c_str = jstringTostring(env,j_str);
    // 拼接字符串
    strcat(c_str," 广州");
    LOG_D("c_str = %s",c_str);
    // char * 转string
    jstring j_temp = (*env)->NewStringUTF(env,(const char*)c_str);
    return j_temp;
}

/**
 *  C/C++层访问Java对象的属性
 * */
JNIEXPORT jstring JNICALL Java_com_jiangdg_jnilearning_JNIUitls_accessJavaField
        (JNIEnv *env, jobject jobj){
    // 得到Java类JNIUtils.class
    jclass jniutil_cls = (*env)->GetObjectClass(env,jobj);
    // 得到java对象的key属性ID
    jfieldID key_fid = (*env)->GetFieldID(env,jniutil_cls,"key","Ljava/lang/String;");
    // 得到Java对象Key属性的值
    jstring key = (jstring)(*env)->GetObjectField(env,jobj,key_fid);
    // 拼接一个新的c字符串
    const char * c_key = (*env)->GetStringUTFChars(env,key,JNI_FALSE);
    char  c_temp[100] = "Hello,";
    strcat(c_temp,c_key);
    // 修改key属性的值
    jstring j_temp = (*env)->NewStringUTF(env,c_temp);
    (*env)->SetObjectField(env,jobj,key_fid,j_temp);
    return j_temp;
}

/**
 *  C/C++层访问Java类的静态属性
 * */
JNIEXPORT void JNICALL Java_com_jiangdg_jnilearning_JNIUitls_accessJavaStaticField
        (JNIEnv *env, jobject jobj){
    jclass ju_cls = (*env)->GetObjectClass(env,jobj);
    // 得到JNIUtils类静态属性count的ID
    jfieldID count_fid = (*env)->GetStaticFieldID(env,ju_cls,"count","I");
    // 得到count属性的值
    jint count = (*env)->GetStaticIntField(env,ju_cls,count_fid);
    // 修改count属性的值
    jint new_count = count+1;
    (*env)->SetStaticIntField(env,ju_cls,count_fid,new_count);
}

/**
 *  C/C++层访问Java对象的方法
 * */
JNIEXPORT void JNICALL Java_com_jiangdg_jnilearning_JNIUitls_accessJavaMethod
        (JNIEnv *env, jobject jobj){
    jclass cls = (*env)->GetObjectClass(env,jobj);
    // 得到JNIUtils类对象genRandomInt方法的ID
    jmethodID mid = (*env)->GetMethodID(env,cls,"genRandomInt","(I)I");
    // 调用genRandomInt方法
    jint random = (*env)->CallIntMethod(env,jobj,mid,200);
    LOG_I("genRandomInt() = %d",random);
}

/**
 * C/C++层访问Java类的静态方法
 * */
JNIEXPORT void JNICALL Java_com_jiangdg_jnilearning_JNIUitls_accessJavaStaticMethod
        (JNIEnv *env, jobject jobj){
    jclass ju_cls = (*env)->GetObjectClass(env,jobj);
    // 得到JNIUtils类getUUID静态方法ID
    jmethodID mid = (*env)->GetStaticMethodID(env,ju_cls,"getUUID","()Ljava/lang/String;");
    // 调用getUUID方法
    jstring UUID = (jstring)(*env)->CallStaticObjectMethod(env,ju_cls,mid);
    LOG_I("getUUID() = %s",(*env)->GetStringUTFChars(env,UUID,JNI_FALSE));
}

/**
 * C/C++访问Java的父类方法
 * */
JNIEXPORT void JNICALL Java_com_jiangdg_jnilearning_JNIUitls_accessJavaFatherMethod
        (JNIEnv *env, jobject jobj){
    jclass cls = (*env)->GetObjectClass(env,jobj);
    // 得到JNIUtils对象fruitInfo属性对象的ID
    jfieldID fid =  (*env)->GetFieldID(env,cls,"fruitInfo","Lcom/jiangdg/jnilearning/Fruit;");
    // 得到fruitInfo属性对象
    jobject fruit_jobj = (*env)->GetObjectField(env,jobj,fid);
    // 得到父类Fruit
    jclass fruit_cls = (*env)->FindClass(env,"com/jiangdg/jnilearning/Fruit");
    // 得到父类的printEatInfo方法，调用该方法
    jmethodID fruit_mid = (*env)->GetMethodID(env,fruit_cls,"printEatInfo","()V");
    (*env)->CallNonvirtualVoidMethod(env,fruit_jobj,fruit_cls,fruit_mid);
}

JNIEXPORT void JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getChineseFromC1
        (JNIEnv *env, jobject jobj, jstring j_str){
    unsigned char qm, wm;					// 区码、位码
    unsigned int i,offset = 0;					// 偏移量
    FILE *fp = NULL;
    //----------------------------------------------------------------------------
    // 得到UTF-8格式的字节数组数据
    const char* c_str_utf8 = (*env)->GetStringUTFChars(env,j_str,JNI_FALSE);
//    // 使用iconv库将UTF-8转换为GB2312
//	iconv_t cd = iconv_open ("gbk","utf-8");
//	if(cd == (iconv_t)-1){
//		LOG_E("打开iconv库失败，error = %d",errno);
//		return;
//	}
//	// 转换汉字编码格式
//	char* c_str_gbk = (char *)malloc(32);
//	unsigned int len = 32;
//	char *temp = (char *)malloc(32);
//	strcpy(temp,c_str_utf8);
//	iconv (cd, &temp,&len, &c_str_gbk, &len);
//	iconv_close (cd);
    //----------------------------------------------------------------------------
    // 从字库中查找
    // 打开HZK16字库文件，提取"蒋"字模，汉字占两个字节，取其区位号
    if ((fp = fopen("/storage/emulated/0/HZK16c", "rb")) == NULL) {
        LOGE("打开HZK16字库文件失败，请查看文件是否存在");
        return;
    }
    // 计算<蒋>在字库中的偏移量
    qm = *c_str_utf8 - 0xa0;
    wm = *(c_str_utf8+1) - 0xa0;
    offset = (94*(qm-1)+(wm-1))*32;
    LOG_D("区位=%d--->码位=%d--->偏移量=%d",qm,wm,offset);
    // 开辟一段内存空间，根据偏移量在字库中定位，提取汉字字模
    char *table = (char *)malloc(32);
    fseek(fp, offset, SEEK_SET);
    fread(table,1,32,fp);
    // 打印序列
    LOGD("提取<蒋>字模数据：");
    for(i=0;i<32;i++){
        LOG_I("%#x,",table[i]);
    }
    fclose(fp);
    fp = NULL;
}

JNIEXPORT jstring JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getChineseFromC2
        (JNIEnv *env, jobject jobj){
    // (1) 得到String(byte[] data,String charset)构造方法ID
    jclass str_cls = (*env)->FindClass(env,"java/lang/String");
    jmethodID construct_mid = (*env)->GetMethodID(env,str_cls,"<init>","([BLjava/lang/String;)V");
    // (2) 得到编码为GB2312 字符数组
    const char* c_str = "Hello，中国！";
    jbyteArray bArray = (*env)->NewByteArray(env,strlen(c_str));
    (*env)->SetByteArrayRegion(env,bArray,0,strlen(c_str),(const signed char *)c_str);
    // (3) 指定构造String编码 UTF-8
    jstring j_charset = (*env)->NewStringUTF(env,"UTF-8");
    // 将GB2312编码的字符串转换为UTF-8，返回给Java
    jstring result = (jstring)(*env)->NewObject(env,str_cls,construct_mid,bArray,j_charset);
    return result;
}

JNIEXPORT jintArray JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getByteArrayFromC(
        JNIEnv *env, jobject jobj) {
    jsize i,len = 10;
    // 创建一个jintArray
    jintArray j_arr = (*env)->NewIntArray(env,len);
    // jnitArray转jint *(即c/c++)
    jint* c_arr = (*env)->GetIntArrayElements(env,j_arr,JNI_FALSE);
    // 写入数据
    for(i=0;i<len;i++){
        *(c_arr + i) = i;
    }
    // 同步到Java层
    (*env)->ReleaseIntArrayElements(env,j_arr,c_arr,JNI_COMMIT);
    return j_arr;
}

JNIEXPORT jintArray JNICALL Java_com_jiangdg_jnilearning_JNIUitls_getSortedArray(
        JNIEnv *env, jobject jobj, jintArray j_arr) {
    // jintArray转jint *
    jint* c_arr = (*env)->GetIntArrayElements(env,j_arr,JNI_FALSE);
    // 得到数组的长度
    jsize arrLen = (*env)->GetArrayLength(env,j_arr);
    // 排序
    qsort(c_arr,arrLen,sizeof(jint),compare);
    // 同步数据到Java层
    (*env)->ReleaseIntArrayElements(env,j_arr,c_arr,JNI_COMMIT);
    return j_arr;
}

int compare(const void *a,const void *b){
    return *(int *)a - *(int *)b;
}

char* jstringTostring(JNIEnv* env, jstring j_str) {
    char* c_str = NULL;
    jclass clsstring = (*env)->FindClass(env,"java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env,"utf-8");
    jmethodID mid = (*env)->GetMethodID(env,clsstring, "getBytes","(Ljava/lang/String;)[B");
    jbyteArray byteArr = (jbyteArray)(*env)->CallObjectMethod(env,j_str, mid,strencode);
    jsize arrLen = (*env)->GetArrayLength(env,byteArr);
    jbyte* ba = (*env)->GetByteArrayElements(env,byteArr, JNI_FALSE);
    if (arrLen > 0) {
        c_str = (char*) malloc(arrLen + 1);
        memcpy(c_str, ba, arrLen);
        c_str[arrLen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env,byteArr, ba, 0);
    return c_str;
}









































