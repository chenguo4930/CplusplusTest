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
    jint *elems = env->GetIntArrayElements(jint_arr,NULL);
    int i = 0;
    for (; i < len; ++i) {
        elems[i] = i;
    }

    //同步
    env->ReleaseIntArrayElements(jint_arr,elems, 0);
    return jint_arr;
}

//JNI 引用变量
//引用类型:局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

extern "C" jintArray Java_com_example_cheng_cplusplustest_MainActivity_localRef
        (JNIEnv *env, jobject jobj) {

}































