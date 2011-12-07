#ifndef GLUTSHADOW_HPP
#define GLUTSHADOW_HPP

namespace GLUT
{

    class GLUTWidget;               // 前方宣言(相互参照だから)
	class GLUTShadow
	{
	private:
		int texWidth_;               // テクスチャ座標
	    int texHeight_;              // テクスチャ座標 
		bool isUseAlpha_;            // アルファテストを使うか？

    	float  angle_;               // ライトからシャドウマップを作る範囲のパースペクティブの範囲の角度　
    	float  nearDistance_;        // パースペクティブの領域の光源に近い面までの距離　 
	    float  farDistance_;         // パースペクティブの領域の光源に遠い面までの距離　 　 
    	bool isLoaded_;
	    bool isInitialized_;
        bool isEnabled_;
    	//ViewGL* view_P;
	    GLUTWidget* glutWidget_;     //glutWidget 
	public:
    	//void ( *scene_P ) ();// シーンをレンダリングする関数へのポインタ
	    GLUTShadow();    // Shadowクラスのコンストラクタ
		void Load(GLUTWidget* glutWidget, bool isUseAlpha = true, float angle = 60, float near = 2.0, float far = 20.0 );
		void drawScene();
		void Initialize();

    // >アクセッサ
		int getTexWidth(){return texWidth_;}
		int getTexHeight(){return texHeight_;}
		bool getIsUseAlpha(){return isUseAlpha_;}
		float getAngle(){return angle_;}
		float getNearDistance(){return nearDistance_;}
		float getFarDistance(){return farDistance_;}
		bool getIsLoaded(){return isLoaded_;}
		bool getIsInitialized(){return isInitialized_;}
		bool getIsEnagled(){return isEnabled_;}

	// >ミューテータ
		void setTexWidth(int texWidth){texWidth_ = texWidth;}
		int setTexHeight(int texHeight){texHeight_ = texHeight;}
		bool setIsUseAlpha(bool isUseAlpha){isUseAlpha_ = isUseAlpha;}
		float setAngle(float angle){angle_ = angle;}
		float setNearDistance(float nearDistance){nearDistance_ = nearDistance;}
		float setFarDistance(float farDistance){farDistance_ = farDistance;}
		void setIsEnabled(bool isEnabled){isEnabled_ = isEnabled;}

	private:
		void Init();
		void createShadowMap(void);          // 床井さんの実装を参考に作成。3パスでのシャドウマップを実現

	}; // end class Shadow
} // end namespace GLUT

#endif