#include "Sound.h"
#include "Window.h"

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
// mmioで必要
#pragma comment(lib,"winmm.lib")

Sound* Sound::p_instance = nullptr;
LPDIRECTSOUND8 Sound::m_sound_interface = nullptr;

bool Sound::InitSound()
{
	// DirectSoundインターフェイスを作成する
	/*
		第一引数：サウンドデバイスを示すGUID
			NULL						：デフォルトのデバイス
			DSDEVID_DefaultPlayback		：デフォルトのデバイス
			DSDEVID_DefaultVoicePlayback：デフォルトの音声再生デバイス

		第二引数：作成されたインターフェイスを保存する変数ポインタ

		第三引数：NULL固定
	*/
	if (FAILED(DirectSoundCreate8(
		NULL,				// GUID
		&m_sound_interface,	// 生成したDirectSound保存先
		NULL				// NULL固定
	)))
	{
		return false;
	}

	// 協調レベルの設定
	/*
		標準協調レベル(DSSCL_NORMAL)
			※この設定は固定なので、プログラム中では変更不可
			下記のプライマリバッファ使用する
				・22KHz
				・ステレオ
				・8ビットサンプリング

		優先協調レベル(DSSCL_PRIORITY)
			標準協調レベルよりもプライマリバッファに対して設定できる項目が増える
			サンプリングレートと深度ビットをアプリ側で設定可能

		書き込み優先レベル(DSSCL_WRITEPRIMARY)
			プライマリバッファに直接アクセスできるようになるが、
			セカンダリバッファを使用することができなくなる
			※サウンドに対しての知識と技術が必要になってくる
	*/
	if (FAILED(m_sound_interface->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		DSSCL_NORMAL
	)))
	{
		return false;
	}

	return true;
}

bool Sound::LoadSound(std::string file_name_, std::string file_list_name_)
{
	// セカンダリバッファの作成
	/*
		作成の手順
		1.サウンドファイルの読み込み
			セカンダリバッファの役割は保存されたサウンドデータを使って音を鳴らす
			そのために、バッファにサウンドデータを保存させる

			※サウンドファイルの読み込みはDirectSoundではサポートしていない
			　開発側でファイルを読み込まなければならない
			※バッファとして使用できるのはWAVファイルのPCM形式
			　読み込む際はWAVファイルか、MP3、OGGをデコードしてPCM形式にする必要がある

		2.バッファ作成

		3.サウンドデータの反映
	*/

	// すでに読み込んでいるか調べる
	// 読み込まれているならfalse、読み込まれていないならtreu
	if (HasKeyName(file_list_name_) == true)
	{
		return false;
	}

	// WAVファイル読み込み
	WavData wav_data;
	if (LoadWavFile(file_name_, &wav_data) == false)
	{
		return false;
	}

	// バッファの作成①
	/*
		バッファの作成はDSBUFFERDESC構造体で設定を行う
		DirectSoundのバッファの情報設定用構造体
			・dwSize：構造体のサイズ
			・dwFlags：サウンドバッファの設定フラグで組み合わせて使用する
				※一部
				DSBCAPS_CTRLPAN			：音源を左右に移動できる
				DSBCAPS_CTRLVOLUME		：ボリュームを変更できる
				DSBCAPS_CTRLFREQUENCY	：周波数を変更できる
			・dwBufferBytes：作成するバッファのサイズ
			・dwReserved：予約0固定
			・lpwfxFormat：波形フォーマット
			・guid3DAlgorithm：DirectSound3Dのハードウェアシュミュレーションで使う
				スピーカー仮想化アルゴリズム
				2Dサウンドの場合はDS3DALG_DEFAULTで問題ない


			ポイントとなるメンバ
				dwFlags
					作成されたバッファが変更できる項目の指定をする
				dwBufferBytes
					バッファのサイズ
					サウンドデータのサイズを指定、サウンドファイルの読み込みで取得可能
				lpwfxFormat
					WAVフォーマット情報
					サウンドファイルの読み込みで取得可能
					WindowsでWAVファイルを取り扱うmmioというAPIがあるが、
					そのAPIで用意されているWAVEFORMATEX構造体を指定する
					WAVEFORMATEX構造体の変数をすべて開発側で設定しても有効なら問題なし
	*/
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;	// DSBCAPS_CTRLPAN：パンの変更 | DSBCAPS_CTRLVOLUME：ボリュームの変更
	dsbd.dwBufferBytes = wav_data.size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.wav_format;

	// バッファの作成②
	/*
		IDirectSoundBufferのCreateSoundBuffer関数で作成する
		DSBUFFERDESCの設定でサウンドバッファを作成
			第一引数：作成するバッファ情報
			第二引数：作成されたバッファの保存先
			第三引数：NULL固定
	*/
	if (FAILED(m_sound_interface->CreateSoundBuffer(
		&dsbd,									// バッファ情報
		&m_sound_buffer_list[file_list_name_],	// 作成されたバッファの保存先
		NULL
	)))
	{
		delete[] wav_data.sound_buffer;
		return false;
	}

	// サウンドデータの反映
	/*
		バッファにサウンドデータの反映をする
		バッファの反映はIDirectSoundBufferのLock関数でアクセス許可を得てから
		データを反映し、Unlock関数でアクセス許可を解除する
		※この2つの関数はセットとして使うこと
	*/
	// 波形データを書き込むためのバッファとサイズ
	void* buffer;
	DWORD buffer_size;

	// Lock
	/*
		指定した範囲でバッファをロックする
		バッファのサイズによって、ロックするバッファが2つのパートに分けられるが
		第五引数、第六引数にNULLを指定すれば分けられることはない

		第一引数：バッファのオフセット指定
		第二引数：バッファのサイズ
		第三引数：ロックされたバッファパート1の保存先変数
		第四引数：バッファパート1のサイズ
		第五引数：バッファパート2のサイズ
		第六引数：ロックのオプション設定
			・DSBLOCK_FROMWRITECURSOR
				カレントポジションからロックする
			・DSBLOCK_ENTIREBUFFER
				バッファ全体をロックする
				※dwBytesは無視される
	*/
	if (FAILED(m_sound_buffer_list[file_list_name_]->Lock(
		0,				// オフセット
		wav_data.size,	// ロックするバッファサイズ
		&buffer,		// ロックされたバッファパート1の保存先
		&buffer_size,	// ロックされたバッファパート1のサイズ
		NULL,			// ロックされたバッファパート2の保存先
		NULL,			// ロックされたバッファパート2のサイズ
		0				// ロックオプション
	)))
	{
		delete[] wav_data.sound_buffer;
		return false;
	}

	// バッファにデータを反映
	memcpy(buffer, wav_data.sound_buffer, buffer_size);

	// Unlock
	/*
		ロックされたバッファをアンロックする

		第一引数：アンロックするバッファパート1
		第二引数：バッファパート1のサイズ
		第三引数：アンロックするバッファパート2
		第四引数：バッファパート2のサイズ
	*/
	m_sound_buffer_list[file_list_name_]->Unlock(
		&buffer,		// アンロックするバッファパート1
		buffer_size,	// パート1のバッファサイズ
		NULL,			// アンロックするバッファパート2
		NULL			// パート2のバッファサイズ
	);

	// コピーが終わったのでサウンドデータを解放
	delete[] wav_data.sound_buffer;

	return true;
}

void Sound::PlaySoundFile(std::string file_list_name_, bool is_loop_, int volume_)
{
	if (m_sound_buffer_list[file_list_name_] == nullptr)
	{
		return;
	}

	// ? 音に変化がない ?
	//m_sound_buffer_list[file_list_name_]->SetVolume(volume_);

	// 再生する
	/*
		第一引数：予約引数　常に0
		第二引数：優先順位
				　0(最低)～0xffffffff(最大)
		第三引数：バッファの再生方法の指定
				　フラグの種類が複数ある、組み合わせて使う

				 DSBPLAY_LOOPING：再生されたサウンドにループ設定を追加する
	*/
	int loop_bit = is_loop_ == true ? 1 : 0;
	m_sound_buffer_list[file_list_name_]->Play(
		0,							// 常に0
		0,							// 優先順位
		DSBPLAY_LOOPING & loop_bit	// ループ設定
	);
}

void Sound::StopSoundFile(std::string file_list_name_)
{
	if (m_sound_buffer_list[file_list_name_] == nullptr)
	{
		return;
	}

	// 停止
	m_sound_buffer_list[file_list_name_]->Stop();
	// 再生位置を先頭に戻す
	/*
		Stop関数は再生を止めるだけなので
		バッファの位置が変わらない

		音を最初から再生したいときに使う
		引数：0を指定してやる
	*/
	m_sound_buffer_list[file_list_name_]->SetCurrentPosition(NULL);
}

void Sound::ReleaseSound()
{
	// セカンダリバッファの解放

	for (auto& list : m_sound_buffer_list)
	{
		if (list.second != nullptr)
		{
			list.second->Stop();
			list.second->Release();
			list.second = nullptr;
		}
	}
	m_sound_buffer_list.clear();

	// DirectSoundインターフェースの解放
	if (m_sound_interface != nullptr)
	{
		m_sound_interface->Release();
		m_sound_interface = nullptr;
	}
}

bool Sound::LoadWavFile(const std::string file_name_, WavData* out_wav_data_)
{
	// WAVファイルの操作方法
	/*
		WAVファイルの操作はWinAPIのmmio(Multi Media IO)を使用する
		mmioはRIFF形式のファイルを操作する関数が用意されている

		ファイルの読み書きで使用するファイル操作の一覧
		1.ファイルのオープンとクローズ
		2.チャンクへの進入と退出
		3.チャンクの読み込み
		4.チャンクの書き込み

		WAVファイルの読み込み手順
		1.ファイルを開く
		2.RIFFチャンクへ進入する
		3.チャンクへ進入する
		4.チャンクで読み書きをする
		5.チャンクから退出する
		6.3～5を繰り返す
		7.ファイルを閉じる
		　2～6で失敗の時も行う
	*/

	// WindowsマルチメディアAPIのハンドル
	HMMIO mmio_handle = NULL;

	// チャンク情報
	MMCKINFO ck_info;
	// RIFFチャンク用
	MMCKINFO riffck_info;

	// WAVファイル内の音サンプルのサイズ
	DWORD dw_wav_size = 0;

	// ファイルオープン
	/*
		WAVファイルも通常のファイルと同様でオープンして
		ファイルに対する処理を行い、用が済んだらクローズする

		mmioOpen関数は引数でファイル名とモードの指定を行う
		結果：MMIOの情報を取得
			　HMMIOを取得(MMIOのハンドル)

		第一引数：開く予定のファイル名
		第二引数：MMIOINFOのポインタ
				　DirectSoundの読み込みでは不要なのでNULL
		第三引数：開いた後のファイルの扱い方の設定
			MMIO_READ：読み込み専用
			MMIO_WRITE：書き込み専用
			MMIO_READWRITE：読み書き可
			※他にもオプション設定はある
	*/
	mmio_handle = mmioOpenA(
		(char*)file_name_.c_str(),	// ファイル名
		NULL,						// MMIO情報
		MMIO_READ					// モード
	);
	if (mmio_handle == NULL)
	{
		return false;
	}

	// dataフォーマットへの進入設定
	/*
		mmioFOURCCはマクロになっている
		引数に4つの文字を指定できる
		この文字をビットシフトでずらして、
		4バイトに1文字ずつ文字が配置された整数を作成している

		このマクロによって、WAVフォーマットの
		ckidやfccTypeの文字が設定された整数を簡単に作成できるようになる
	*/
	// WAVファイルの先頭チャンクであるRIFFチャンクに進入する
	// RIFFチャンクに進入するためにfccTypeにWAVEを設定する
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// チャンクへ進入または退出
	/*
		WAVファイルはチャンクごとにデータが分かれているのでアクセスするチャンクを決める
		これを「進入」とする
		別のチャンクにアクセスするときは選択中のチャンクをキャンセルしなければならない
		これを「退出」とする

		チャンクへの進入：mmioDescend関数
			指定されたチャンクに進入する

			第一引数：進入対象のチャンクを持つMMIOハンドル
			第二引数：進入したチャンク情報
			第三引数：親チャンク
			第四引数：進入するチャンクの種類

			※第二引数について
				この引数は入力と出力を兼ねている
				mmioDescend関数は第二引数のMMCKINFOの「ckid」「fccType」と
				第四引数の情報の種類の組み合わせで進入するチャンクを探している
				(組み合わせはyttmのサウンドで)

		チャンクから退出：mmioAscend関数
			チャンクを退出する(上の階層に戻る)

			第一引数：退出対象もMMIOハンドル
			第二引数：退出先のチャンク情報
			第三引数：退出先のチャンク情報

			ハンドルと進入中のチャンクを指定すればチャンクから退出できる
	*/
	// RIFFチャンクに進入する
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&riffck_info,
		NULL,
		MMIO_FINDRIFF
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 最初はWAVファイルのサンプリングレートなどを保持している「fmtチャンク」に進入する
	// 進入先のチャンクを"fmt"として設定する
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&ck_info,
		&riffck_info,
		MMIO_FINDCHUNK
	))
	{
		// fmtチャンクがない
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// チャンクの読み込み
	/*
		チャンクデータの読み込みは進入中のチャンクに対してのみ行える
		※mmioOpenで読み書きの設定をしているが、この設定に反した処理ができない
		　(読み込み専用でファイルを開いている場合は、書き込みができない)

		第一引数：取得対象のMMIOハンドル
		第二引数：チャンクを読み込んだデータを保存するバッファ
		第三引数：バッファのサイズ
	*/
	// fmtチャンクを読み込む
	LONG read_size = mmioRead(
		mmio_handle,						// ハンドル
		(HPSTR)&out_wav_data_->wav_format,	// 読み込み先のバッファ
		sizeof(out_wav_data_->wav_format)	// バッファのサイズ
	);
	if (read_size != sizeof(out_wav_data_->wav_format))
	{
		/*
			サイズが一致しないのでエラー
				第三引数で読み込んだバッファのサイズが戻り値で確保できているが、
				これが構造体とサイズが違うとき、問題があるということ
		*/
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットのチェック
	if (out_wav_data_->wav_format.wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 読み込みが完了したので退出する
	if (MMSYSERR_NOERROR != mmioAscend(
		mmio_handle,
		&ck_info,
		0
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// 音データを保持しているdataチャンクに進入する
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&ck_info,
		&riffck_info,
		MMIO_FINDCHUNK
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// サイズを保存
	out_wav_data_->size = ck_info.cksize;

	// 音データを読み込む
	out_wav_data_->sound_buffer = new char[ck_info.cksize];
	read_size = mmioRead(
		mmio_handle,
		(HPSTR)out_wav_data_->sound_buffer,
		ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wav_data_->sound_buffer;
		return false;
	}



	// チャンクの書き込み
	/*
		チャンクデータに書き込むのはチャンクに進入していなくても可能
		※mmioOpenで読み書きの設定をしているが、この設定に反した処理ができない
		　(読み込み専用でファイルを開いている場合は、書き込みができない)

		 第一引数：取得対象のMMIOハンドル
		 第二引数：ファイルに書き込むバッファ
		 第三引数：バッファのサイズ
	*/
	//mmiowrite(
	//	mmio_handle,	// ハンドル
	//	buffer,			// 書き込みバッファ
	//	buffer_size		// バッファのサイズ
	//);

	// ファイルクローズ
	/*
		ファイルを閉じる

		第一引数：閉じる予定のmmioハンドル
		第二引数：オプション
				　MMIO_FHOPEN
					第一引数がMMIOファイル以外が指定された場合は無視して、
					MMIOファイルだけを閉じるようにする
	*/
	mmioClose(
		mmio_handle,	// クローズ対象のハンドル
		MMIO_FHOPEN		// オプション(設定なしは0)
	);

	return true;
}

bool Sound::HasKeyName(std::string file_list_name_)
{
	auto it = m_sound_buffer_list.find(file_list_name_);
	if (it != m_sound_buffer_list.end())
	{
		return true;
	}
	return false;
}