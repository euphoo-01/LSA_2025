#include "stdafx.h"
#include "RPN.h"

using namespace std;

namespace RPN
{
	int getPriority(LT::Entry cur_lex)
	{
		switch (cur_lex.lexema[0])
		{
		case LEX_STAR:
			return 3;
		case LEX_DIRSLASH:
			return 3;
		case LEX_OST:
			return 3;
		case LEX_PLUS:
			return 2;
		case LEX_MINUS:
			return 2;
		case LEX_AND:
			return 2;
		case LEX_OR:
			return 2;
		case LEX_COMMA:
			return 1;
		case LEX_LEFTTHESIS:
			return 0;
		case LEX_RIGHTTHESIS:
			return 0;
		default:
			return -1;
		}
	}
	void Rpn(LT::LexTable lextable, IT::IdTable idtable, int curPos)
	{
		queue<LT::Entry> q;
		stack<LT::Entry> s;
		LT::Entry bottom = { '$', LT_TI_NULLIDX, LT_TI_NULLIDX };
		int curpos = curPos;
		int kolLex = 0;
		for (curPos; lextable.table[curPos].lexema[0] != LEX_SEMICOLON; curPos++, kolLex++)
		{
			switch (lextable.table[curPos].lexema[0])
			{
			case LEX_ID:
			{
				if (idtable.table[lextable.table[curPos].idxTI].idtype == IT::IDTYPE::F)
				{
					s.push(lextable.table[curPos]);
					continue;
				}
				q.push(lextable.table[curPos]);
				continue;
			}
			case LEX_LITERAL:
			{
				q.push(lextable.table[curPos]);
				continue;
			}
			case LEX_LEFTTHESIS:
			{
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_RIGHTTHESIS:
			{
				if (!s.empty())
				{
					while (s.top().lexema[0] != LEX_LEFTTHESIS)
					{
						q.push(s.top());
						s.pop();
					}
					s.pop();
				}
				continue;
			}
			case LEX_STAR: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_DIRSLASH: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_OST: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_PLUS: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_MINUS: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_AND: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_OR: {
				if (!s.empty())
				{
					while (getPriority(lextable.table[curPos]) <= getPriority(s.top()) || idtable.table[s.top().idxTI].idtype == IT::IDTYPE::F)
					{
						q.push(s.top());
						s.pop();
						if (s.empty())
							break;
					}
				}
				s.push(lextable.table[curPos]);
				continue;
			}
			case LEX_COMMA:{
				while (s.top().lexema[0] != LEX_LEFTTHESIS)
				{
					q.push(s.top());
					s.pop();
				}
				continue;
			}
			default:
				continue;
			}
		}
		while (!s.empty())
		{
			q.push(s.top());
			s.pop();
		}

		for (int i = 0; i < kolLex; i++)
		{
			if (!q.empty())
			{
				switch (q.front().lexema[0])
				{
				case LEX_ID:
					if (curpos <= idtable.table[q.front().idxTI].idxfirstLE && idtable.table[q.front().idxTI].idxfirstLE < curpos + kolLex)
					{
						lextable.table[curpos + i] = q.front();
						idtable.table[q.front().idxTI].idxfirstLE = curpos + i;
						q.pop();
					}
					else
					{
						lextable.table[curpos + i] = q.front();
						q.pop();
					}
					continue;
				case LEX_LITERAL:
					lextable.table[curpos + i] = q.front();
					idtable.table[q.front().idxTI].idxfirstLE = curpos + i;
					q.pop();
					continue;
				default:
					lextable.table[curpos + i] = q.front();
					q.pop();
					continue;
				}
			}
			else
			{
				lextable.table[curpos + i] = bottom;
			}
		}
	}
	void searchNextPosForCheck(LT::LexTable lextable, IT::IdTable idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_EQUAL)
			{
				Rpn(lextable, idtable, ++i);
			}
			else if (lextable.table[i].lexema[0] == LEX_WHILE || lextable.table[i].lexema[0] == LEX_IF) {
				if (lextable.table[i + 3].lexema[0] == LEX_OR || lextable.table[i + 3].lexema[0] == LEX_AND) {
					swap(lextable.table[i + 3].lexema[0], lextable.table[i + 4].lexema[0]);
				}
			}
		}
	}
}