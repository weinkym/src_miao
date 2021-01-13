import QtQuick 2.12

//自定义的环形进度条组件
//没设置OpenGL版本，可能默认时OpenGL ES2.0吧
ShaderEffect{
    id: control

    property double min: 0
    property double max: 100
    property double value: 0
    // 给着色器的
    property real aValue: (value-min)/(max-min)
    // 修改值后触发动画效果
    Behavior on aValue{
        NumberAnimation{
            duration: 1000
        }
    }

    // 上面的文字
    Text{
        anchors.centerIn: parent
        color: "black"
        font.pixelSize: 20
        text: Number(aValue*100).toFixed(2)+" %"
    }

    //mat4 qt_Matrix组合的转换矩阵，从根项到此ShaderEffect的矩阵与正交投影的乘积
    //vec4 qt_Vertex顶点位置，左上角顶点的位置为（0，0），右下角顶点的位置为（width，height）
    //vec2 qt_MultiTexCoord0纹理坐标，左上角坐标为（0，0），右下角坐标为（1，1）
    //thePos用来指定位置，标准化设备坐标
    //aSmoothWidth用来指定smoothstep的宽度
    vertexShader: "
            uniform mat4 qt_Matrix;
            attribute vec4 qt_Vertex;
            attribute vec2 qt_MultiTexCoord0;
            varying vec2 thePos;
            varying float aSmoothWidth;
            uniform float width;
            void main() {
                thePos = vec2(qt_MultiTexCoord0.x*2.0-1.0,-qt_MultiTexCoord0.y*2.0+1.0);
                aSmoothWidth = 3.0/float(width);
                gl_Position = qt_Matrix * qt_Vertex;
            }"

    //float qt_Opacity从根项到此ShaderEffect的不透明度的乘积。
    //es2里字面量也得写成和运算变量一样的类型，如浮点数
    //myatan2把角度归一化为[0,1]
    fragmentShader: "
            #define PI 3.14159265
            varying vec2 thePos;
            varying float aSmoothWidth;
            uniform float qt_Opacity;
            uniform float aValue;
            float myatan2(float y,float x)
            {
                float ret_val = 0.0;
                if(x != 0.0){
                    ret_val = atan(y,x);
                    if(ret_val < 0.0){
                        ret_val += 2.0*PI;
                    }
                }else{
                    ret_val = y>0.0 ? PI*0.5 : PI*1.5;
                }
                return ret_val/(2.0*PI);
            }
            void main() {
                float len = abs(sqrt(pow(thePos.x,2.0)+pow(thePos.y,2.0)));
                float alpha = 1.0-smoothstep(0.15,0.15+aSmoothWidth,abs(len-0.75));
                float angle = myatan2(thePos.y,thePos.x);
                float angle_smooth=1.0-smoothstep(aValue,aValue+aSmoothWidth/3.0,angle);
                if(angle_smooth>0.0){
                if(angle_smooth>=1.0){
                gl_FragColor = vec4(1.0,0.1,(1.0-angle),alpha);
                }else{
                gl_FragColor = vec4(mix(vec3(1.0,0.1,(1.0-angle)),vec3(0.4,0.1,0.6),1.0-angle_smooth),alpha);
                }
                }else{
                gl_FragColor = vec4(0.4,0.1,0.6,alpha);
                }
            }"
}
