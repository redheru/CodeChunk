//-----------------------------------------------------------------------------
//                             MotionData.h
//
// モーションデータクラス宣言部
//
// $File: $
// $Author: numaguchi $
// $Date: 2011/10/30 $
// $Revision: 1$
//
//
//-----------------------------------------------------------------------------

#ifndef MOTION_DATA_CLASS_HEADER_
#define MOTION_DATA_CLASS_HEADER__
#pragma once

// Eigen Library
#include "EigenMath.h"

// 説明:
// モーションデータクラス
class CMotionData
{
// 説明:
// variables
private:
	//! 関節数
	size_t m_nJoints;
	//! フレーム数
	size_t m_nFrames;
	//! フレーム時間
	float m_nFrameTime;
	//! ルート位置時系列データ
	Vector3f *m_pPosition;
	//! 関節回転角度時系列データ
	Quaternionf *m_pRotation;

// Eigenライブラリの要素を動的に生成するときに必要
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

// 説明:
// constructor/destructor
public:
	// デフォルトコンストラクタ
	CMotionData(void);
	// 初期化子付コンストラクタ
	CMotionData(size_t frames, size_t joints);
	// コピーコンストラクタ
	CMotionData(const CMotionData &src);
	// デストラクタ
	virtual ~CMotionData(void);

// 説明:
// initializer
public:
	// 初期化関数
	bool Initialize(size_t frames, size_t joints);
	// 代入演算子
	CMotionData& operator =(const CMotionData &src);

// 説明:
// memory allocator
private:
	// メモリ領域確保
	bool Allocate(size_t frames, size_t joints);
	// メモリ領域解放
	void Destroy();

// 説明:
// attributes
public:
	// データ利用可能状態
	bool IsActive() const;
	//-----------------------------------------------------------------------------
	// 説明: 総フレーム数の取得
    // 引数: 
	// 返り値:
	// その他: 
	//        総フレーム数
	//-----------------------------------------------------------------------------
	size_t NumFrames() const
		{
			return m_nFrames;
		}
	//-----------------------------------------------------------------------------
	// 説明: 総関節ノードの取得
    // 引数: 
	// 返り値:
	//        総関節ノード数
	// その他: 
	//-----------------------------------------------------------------------------
	size_t NumJoints() const
		{
			return m_nJoints;
		}

// 説明:
// accessor
public:  
	// フレーム時間の設定
	bool SetFrameTime(float frmTime){m_nFrameTime = frmTime; return true;}
	// 回転量の設定
	bool SetRotation(size_t frm, size_t jid, const Quaternionf &q);
	// 回転量の設定
	bool SetRotation(size_t frm, size_t jid, const Matrix4f &m);
	// ルート位置の指定
	bool SetPosition(size_t frm, const Vector3f &v);

	// フレーム時間の取得
    float GetFrameTime() const {return m_nFrameTime;}
	// 回転量の取得
	Quaternionf GetRotation(size_t frm, size_t jid) const;
	// 回転量の取得
	Matrix4f GetRotationMatrix(size_t frm, size_t jid) const;
	// ルート位置の取得
	Vector3f GetPosition(size_t frm) const;
	// ルート位置の取得
	Matrix4f GetPositionMatrix(size_t frm) const;

// 説明:
// macros
private:
	//-----------------------------------------------------------------------------
	// 説明: 値域判定マクロ(フレーム)
    // 引数: 
	//       frm [in] 判定対象フレーム番号
	// 返り値      
	//        true 値域内
	//        false 値域外
	// その他: 
	//-----------------------------------------------------------------------------
	bool IsRange(size_t frm) const
		{
			return frm < m_nFrames;
		}
	//-----------------------------------------------------------------------------
	// 説明: 値域判定マクロ(フレーム&自由度)
    // 引数: 
	//       frm [in] 判定対象フレーム番号
	//       jid [in] 判定対象関節インデクス
	// 返り値      
	//        true 値域内
	//        false 値域外
	// その他: 
	//-----------------------------------------------------------------------------
	bool IsRange(size_t frm, size_t jid) const
		{
			return frm < m_nFrames && jid < m_nJoints;
		}
	//-----------------------------------------------------------------------------
	// 説明: 回転データアクセスマクロ
    // 引数: 
	//       frm [in] フレーム番号
	//       jid [in] 関節インデクス
	// 返り値      
	//        回転データへの参照
	// その他: 
	//-----------------------------------------------------------------------------
	Quaternionf& ROT(size_t frm, size_t jid)
		{
			return *(m_pRotation + m_nJoints * frm + jid);
		}
	//-----------------------------------------------------------------------------
	// 説明: 回転データアクセスマクロ(const)
    // 引数: 
	//       frm [in] フレーム番号
	//       jid [in] 関節インデクス
	// 返り値      
	//       回転データへの参照(const)
	// その他: 
	//-----------------------------------------------------------------------------
	const Quaternionf& ROT(size_t frm, size_t jid) const
		{
			return *(m_pRotation + m_nJoints * frm + jid);
		}
	//-----------------------------------------------------------------------------
	// 説明: ルート位置データアクセスマクロ
    // 引数: 
	//       frm [in] フレーム番号
	// 返り値      
	//       位置データへの参照(const)
	// その他: 
	//-----------------------------------------------------------------------------
	Vector3f& POS(size_t frm)
		{
			return *(m_pPosition + frm);
		}
	//-----------------------------------------------------------------------------
	// 説明: ルート位置データアクセスマクロ(const)
    // 引数: 
	//       frm [in] フレーム番号
	// 返り値      
	//       位置データへの参照(const)
	// その他: 
	//-----------------------------------------------------------------------------
	const Vector3f& POS(size_t frm) const
		{
			return *(m_pPosition + frm);
		}
};

#endif //MOTION_DATA_CLASS_HEADER__
