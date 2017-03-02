#include <jni.h>
#include <string>
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
    char filename[100];
    sprintf(filename, "D://%s.txt", uuid_str);
    FILE *fp = fopen(filename, "w");
    fputs("i love guo", fp);
    fclose(fp);
}


