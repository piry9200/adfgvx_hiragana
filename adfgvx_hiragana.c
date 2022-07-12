// 'A'はA\0を組み合わせたもの. 'A'はA. c言語は'と'を区別する.
// textfは空白を取得できない
//ファイルに出力
//復号するときはキーワードを求めるようにする

#include <stdio.h>
#include <wchar.h>
#include <locale.h> 

int itsEndIndexNumIs(wchar_t string[]); //指定したワイド文字列配列の中で一番後ろにある文字のインデックス番号を返す.一文字も入ってない場合-1を返す.
int itsIndexNumIs(wchar_t word); //指定した平仮名がひらがな表の[0][0]の要素を「1」、[8][8]の要素を「81」として、その規則に従った値を返す。見つからなかったら-1を返す。


wchar_t table[9][9] ={
            {L'あ',L'い',L'う',L'え',L'お',L'か',L'き',L'く',L'け'},
            {L'こ',L'さ',L'し',L'す',L'せ',L'そ',L'た',L'ち',L'つ'},
            {L'て',L'と',L'な',L'に',L'ぬ',L'ね',L'の',L'は',L'ひ'},
            {L'ふ',L'へ',L'ほ',L'ま',L'み',L'む',L'め',L'も',L'や'},
            {L'ゆ',L'よ',L'ら',L'り',L'る',L'れ',L'ろ',L'わ',L'を'},
            {L'ん',L'が',L'ぎ',L'ぐ',L'げ',L'ご',L'ざ',L'じ',L'ず'},
            {L'ぜ',L'ぞ',L'だ',L'ぢ',L'づ',L'で',L'ど',L'ば',L'び'},
            {L'ぶ',L'べ',L'ぼ',L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ',L'っ'},
            {L'ゃ',L'ゅ',L'ょ',L'！',L'？',L'「',L'」',L'（',L'）'}
        };


int main(int argc, const char * argv[]){
    setlocale(LC_CTYPE, ""); //ワイド文字列を出力するために必要
    //--------換字をするための準備----------
    wchar_t transformKey[9] = {L'き',L'さ',L'ま',L'み',L'て',L'い',L'る',L'な',L'!'};
    wchar_t arrangeKey[11] = {L"こんばんは"}; //パスワードは10文字以内
    wchar_t text[501] = {L'\0'}; //暗号化する文章を入れるやつ
    int findFlag = 0;
    int indexToInput = 0; //scanfを無限ループして,改行処理をするために使う
    //------------------------------------

    printf("-----------------パスワードを設定してください-----------------\n");
    printf("＊10文字以内\n");
    printf("＊ひらがなのみ\n");
    printf("---------------------------------------------------------\n");
    printf("パスーワード : ");
    // scanf("%ls",arrangeKey);

    //------入力されたパスワードをあいうえお順にソートする------
    wchar_t sort_arrange[11] = {'\0'};
    wchar_t tmp = '\0';
    int preNum = 0; //ひらがな表の位置を入れる
    int postNum = 0; //ひらがな表の位置を入れる
    for(int i=0; i < itsEndIndexNumIs(arrangeKey) + 2; i++){ //バブルソート
        for(int j=0; j < itsEndIndexNumIs(arrangeKey) ; j++){ //　隣同士の比較を行う
            preNum = itsIndexNumis(arrangeKey[j]);
            postNum = itsIndexNumIs(arrangeKey[j+1]); 
            if(preNum > postNum){
                tmp = arrangeKey[i];
                arrangeKey[j] = arrangeKey[j+1];
                arrangeKey[j+1] = tmp;
            }
        }
    }
    //--------------------------------------------------

    //---------転置表をつくるための準備--------- 
    int column = (itsEndIndexNumIs(arrangeKey) + 1 ) * 2; //入力されたパスワードの長さの２倍をnumOfPwdへ代入
    int row = 1000 / (column + 1); //codedTextの 行*列 を1000以上にしたい。列数はパスワードの長さで固定だから、行数は1000をnumOfPwdに１を足したもので割ったものにすればよい
    wchar_t codedText[row][column]; // 
    //------------------------------------

    printf("-----------------文を入力してください。(qで終了)-----------------\n");
    printf("使用可能文字: ひらがな\n");
    printf("使用可能文字: ！, ？, 」,「, ）,（　 ←すべて全角\n");
    printf("改行 : Enterキー\n");
    printf("空白 : _ または ＿\n");
    printf("----------------------↓入力してください------------------------\n");

    while(1){ //scanfを無限ループ
        indexToInput = itsEndIndexNumIs(text); //textfする前の最後尾にある文字のインデックス番号を代入
        scanf("%ls",&text[indexToInput + 1]);
        indexToInput = itsEndIndexNumIs(text); //textf後の最後尾にある文字のインデックス番号を代入
        if(text[indexToInput] == 'q'){ //qが入力されたらループ終了
            break;
        }
        text[indexToInput + 1] = '\n'; //一度Enterキーが押されたら改行する.
    }
    
    for(int i = 0; i < sizeof(text); i++){ //textの文字をひとつずつチェック

        if(text[i] == '\0'){ //textから取り出した値が'\0'だったら全ての文字を見終えたのでtext内の探索を中断する
            printf("\n");
            break;
        } else if(text[i] == '_' || text[i] == L'＿'){
            printf(" ");
            continue;
        } else if(text[i] == '\n'){
            printf("\n");
            continue;
        }

        for(int r = 0; r < 9; r++){  //textから取り出した文字が表の何行何列にあるかを確かめる r=行 c=列
            for(int c = 0; c < 9; c++){
                if(text[i] == table[r][c]){
                    wprintf(L"%lc%lc", transformKey[r], transformKey[c]);
                    findFlag = 1; //表から文字が見つかったらそれを出力してflagを立てる
                    break; //列のインクリメントを中断
                }
            }
            if(findFlag == 1){ //値が見つかったら,行のインクリメントを中断
                findFlag = 0;
                break;
            }
        }
    }
        
    return 0;
}


int itsEndIndexNumIs(wchar_t string[]){
    int count;
    for(count = 0; string[count] != '\0'; count++){

    }
    return count - 1; //配列の最後尾にある文字のインデックス番号を返す.
}

int itsIndexNumis(wchar_t word){
    int count = 0;

     for(int r = 0; r < 9; r++){  //ひらがな表探索
            for(int c = 0; c < 9; c++){ //ひらがな表探索
                count++;
                if(word == table[r][c]){
                    return count;
                }
            }
        }
    
    return -1;
}