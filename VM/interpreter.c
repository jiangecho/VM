#include "interpreter.h"
#include "opcode.h"
#include "stack.h"
#include "attribute.h"
#include "class.h"
#include "method.h"

#include "prepare.h"
#include "resolution.h"
#include "initialize.h"

#include <stdio.h>

static struct frame* pcurrent_frame;
static struct stack* pcurrent_stack;
//static struct method_info* pcurrent_method_info;
//static struct code_attribute_info* pcode_attribute_info;
//static struct Class* pcurrent_class;
u1* pcode;

void update_current_frame()
{
	pcurrent_frame = pcurrent_stack->pcurrent_frame;
}


void set_current_stack(struct stack* pstack)
{
	pcurrent_stack = pstack;
	pcurrent_frame = pstack->pcurrent_frame;
	/*
	pcurrent_frame = pstack->pcurrent_frame;
	pcurrent_method_info = pcurrent_frame->pmethod_info;

	pcode_attribute_info = get_code_attribute_info(pcurrent_method_info);
	pcode = pcode_attribute_info->pcode;
	*/
}

// implement this function as a macro
u1 fetch()
{
	u1 instruction = *pcurrent_frame->pc;
	return instruction;
}

void interpreter()
{
	struct constant_methodref_info* pconstant_methodref_info;
	struct constant_fieldref_info* pconstant_field_info;
	struct constant_name_and_type_info* pconstant_name_and_type_info;
	struct constant_class_info* pconstant_class_info;
	struct Class* pclass;
	u2 index;
	u1 stop = 0;
	u1 ins = fetch();
	// stop will be set when the first method returns
	while(!stop)
	{
		//opcode[fetch()]();
		//TODO
		// handle pc
		switch(ins)
		{
			//Constants
		case   NOP:               //0x00     
			{
				pcurrent_frame->pc ++;
				break;
			}
		case   ACONST_NULL:       //0x01             
			{
				goto unsupport;
			}
		case   ICONST_M1:         //0x02           
			{
				goto unsupport;
			}
		case   ICONST_0:          //0x03          
			{
				*((pcurrent_frame->sp ++)) = 0;
				pcurrent_frame->pc ++;
				break;

			}
		case   ICONST_1:          //0x04          
			{
				*((pcurrent_frame->sp ++)) = 0x00000001;
				pcurrent_frame->pc ++;

				break;
			}
		case   ICONST_2:          //0x05          
			{
				*((pcurrent_frame->sp ++)) = 0x00000002;
				pcurrent_frame->pc ++;

				break;
			}
		case   ICONST_3:          //0x06          
			{
				*((pcurrent_frame->sp ++)) = 0x00000003;
				pcurrent_frame->pc ++;

				break;
			}

		case   ICONST_4:          //0x07          
			{
				*((pcurrent_frame->sp ++)) = 0x00000004;
				pcurrent_frame->pc ++;

				break;
			}
		case   ICONST_5:          //0x08          
			{
				*((pcurrent_frame->sp ++)) = 0x00000005;
				pcurrent_frame->pc ++;

				break;
			}
		case   LCONST_0:          //0x09          
			{
				goto unsupport;
			}
		case   LCONST_1:          //0x0A          
			{
				goto unsupport;
			}
		case   FCONST_0:          //0x0B          
			{
				goto unsupport;
			}
		case   FCONST_1:          //0x0C          
			{
				goto unsupport;
			}
		case   FCONST_2:          //0x0D          
			{
				goto unsupport;
			}
		case   DCONST_0:          //0x0E          
			{
				goto unsupport;
			}
		case   DCONST_1:          //0x0F          
			{
				goto unsupport;
			}
		case   BIPUSH:            //0x10        
			{
				*(pcurrent_frame->sp) = 0x000000FF & *(++pcurrent_frame->pc);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;

				break;
			}

		case   SIPUSH:            //0x11        
			{
				*(pcurrent_frame->sp) = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;

				break;
			}
		case   LDC:               //0x12     
			{
				goto unsupport;
			}
		case   LDC_W:             //0x13       
			{
				goto unsupport;
			}
		case   LDC2_W:            //0x14        

			//loads:
		case   ILOAD:             //0x15       
			{
				u1 index = *(++ pcurrent_frame->pc);
				*pcurrent_frame->sp = *(pcurrent_frame->plocals_start_addr + index);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   LLOAD:             //0x16       
			{
				goto unsupport;
			}
		case   FLOAD:             //0x17       
			{
				goto unsupport;
			}
		case   DLOAD:             //0x18       
			{
				goto unsupport;
			}
		case   ALOAD:             //0x19       
			{
				goto unsupport;
			}
		case   ILOAD_0:           //0x1A         
			{
				*pcurrent_frame->sp = *(pcurrent_frame->plocals_start_addr);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ILOAD_1:           //0x1B         
			{
				*pcurrent_frame->sp = *(pcurrent_frame->plocals_start_addr + 1);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ILOAD_2:           //0x1C         
			{
				*pcurrent_frame->sp = *(pcurrent_frame->plocals_start_addr + 2);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ILOAD_3:           //0x1D         
			{
				*pcurrent_frame->sp = *(pcurrent_frame->plocals_start_addr + 3);
				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   LLOAD_0:           //0x1E         
			{
				goto unsupport;
			}
		case   LLOAD_1:           //0x1F         
			{
				goto unsupport;
			}
		case   LLOAD_2:           //0x20         
			{
				goto unsupport;
			}
		case   LLOAD_3:           //0x21         
			{
				goto unsupport;
			}
		case   FLOAD_0:           //0x22         
			{
				goto unsupport;
			}
		case   FLOAD_1:           //0x23         
			{
				goto unsupport;
			}
		case   FLOAD_2:           //0x24         
			{
				goto unsupport;
			}
		case   FLOAD_3:           //0x25         
			{
				goto unsupport;
			}
		case   DLOAD_0:           //0x26         
			{
				goto unsupport;
			}
		case   DLOAD_1:           //0x27         
			{
				goto unsupport;
			}
		case   DLOAD_2:           //0x28         
			{
				goto unsupport;
			}
		case   DLOAD_3:           //0x29         
			{
				goto unsupport;
			}
		case   ALOAD_0:           //0x2A         
			{
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr);
				*(Object** )(pcurrent_frame->sp) = *pobject;

				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ALOAD_1:           //0x2B         
			{
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 1);
				*(Object** )(pcurrent_frame->sp) = *pobject;

				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ALOAD_2:           //0x2C         
			{
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 2);
				*(Object** )(pcurrent_frame->sp) = *pobject;

				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   ALOAD_3:           //0x2D         
			{
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 3);
				*(Object** )(pcurrent_frame->sp) = *pobject;

				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   IALOAD:            //0x2E        
			{
				goto unsupport;
			}
		case   LALOAD:            //0x2F        
			{
				goto unsupport;
			}
		case   FALOAD:            //0x30        
			{
				goto unsupport;
			}
		case   DALOAD:            //0x31        
			{
				goto unsupport;
			}
		case   AALOAD:            //0x32        
			{
				goto unsupport;
			}
		case   BALOAD:            //0x33        
			{
				goto unsupport;
			}
		case   CALOAD:            //0x34        
			{
				goto unsupport;
			}
		case   SALOAD:            //0x35        

			//stores:
		case   ISTORE:            //0x36        
			{
				*(pcurrent_frame->plocals_start_addr + *(++ pcurrent_frame->pc)) = *(--pcurrent_frame->sp);
				pcurrent_frame->pc ++;
				pcurrent_frame->sp ++;
				break;
			}
		case   LSTORE:            //0x37        
			{
				goto unsupport;
			}
		case   FSTORE:            //0x38        
			{
				goto unsupport;
			}
		case   DSTORE:            //0x39        
			{
				goto unsupport;
			}
		case   ASTORE:            //0x3A        
			{
				goto unsupport;
			}
		case   ISTORE_0:          //0x3B          
			{
				*(pcurrent_frame->plocals_start_addr) = *(--pcurrent_frame->sp);
				pcurrent_frame->pc ++;

				break;
			}
		case   ISTORE_1:          //0x3C          
			{
				*(pcurrent_frame->plocals_start_addr + 1) = *(--pcurrent_frame->sp);
				pcurrent_frame->pc ++;

				break;
			}
		case   ISTORE_2:          //0x3D          
			{
				*(pcurrent_frame->plocals_start_addr + 2) = *(--pcurrent_frame->sp);
				pcurrent_frame->pc ++;

				break;
			}
		case   ISTORE_3:          //0x3E          
			{
				*(pcurrent_frame->plocals_start_addr + 2) = *(--pcurrent_frame->sp);
				pcurrent_frame->pc ++;

				break;
			}
		case   LSTORE_0:          //0x3F          
			{
				goto unsupport;
			}
		case   LSTORE_1:          //0x40          
			{
				goto unsupport;
			}
		case   LSTORE_2:          //0x41          
			{
				goto unsupport;
			}
		case   LSTORE_3:          //0x42          
			{
				goto unsupport;
			}
		case   FSTORE_0:          //0x43          
			{
				goto unsupport;
			}
		case   FSTORE_1:          //0x44          
			{
				goto unsupport;
			}
		case   FSTORE_2:          //0x45          
			{
				goto unsupport;
			}
		case   FSTORE_3:          //0x46          
			{
				goto unsupport;
			}
		case   DSTORE_0:          //0x47          
			{
				goto unsupport;
			}
		case   DSTORE_1:          //0x48          
			{
				goto unsupport;
			}
		case   DSTORE_2:          //0x49          
			{
				goto unsupport;
			}
		case   DSTORE_3:          //0x4A          
			{
				goto unsupport;
			}
		case   ASTORE_0:          //0x4B          
			{
				//TODO implement this instruction
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 0);
				//(Object* )(pcurrent_frame->plocals_start_addr) = (Object* )(--pcurrent_frame->sp);
				*pobject = *(Object** )(--pcurrent_frame->sp);

				pcurrent_frame->pc ++;
				break;
			}
		case   ASTORE_1:          //0x4C          
			{
				//TODO implement this instruction
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 1);
				//(Object* )(pcurrent_frame->plocals_start_addr) = (Object* )(--pcurrent_frame->sp);
				*pobject = *(Object** )(--pcurrent_frame->sp);

				pcurrent_frame->pc ++;
				break;
			}
		case   ASTORE_2:          //0x4D          
			{
				//TODO implement this instruction
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 2);
				//(Object* )(pcurrent_frame->plocals_start_addr) = (Object* )(--pcurrent_frame->sp);
				*pobject = *(Object** )(--pcurrent_frame->sp);

				pcurrent_frame->pc ++;
				break;
			}
		case   ASTORE_3:          //0x4E          
			{
				//TODO implement this instruction
				Object** pobject = (Object** )(pcurrent_frame->plocals_start_addr + 3);
				//(Object* )(pcurrent_frame->plocals_start_addr) = (Object* )(--pcurrent_frame->sp);
				*pobject = *(Object** )(--pcurrent_frame->sp);

				pcurrent_frame->pc ++;
				break;
			}
		case   IASTORE:           //0x4F         
			{
				goto unsupport;
			}
		case   LASTORE:           //0x50         
			{
				goto unsupport;
			}
		case   FASTORE:           //0x51         
			{
				goto unsupport;
			}
		case   DASTORE:           //0x52         
			{
				goto unsupport;
			}
		case   AASTORE:           //0x53         
			{
				goto unsupport;
			}
		case   BASTORE:           //0x54         
			{
				goto unsupport;
			}
		case   CASTORE:           //0x55         
			{
				goto unsupport;
			}
		case   SASTORE:           //0x56         

			//stack:
		case   POP:               //0x57     
			{
				pcurrent_frame->sp --;
				pcurrent_frame->pc ++;
				break;
			}
		case   POP2:              //0x58      
			{
				goto unsupport;
			}
		case   DUP:               //0x59     
			{
				*(pcurrent_frame->sp) = *(pcurrent_frame->sp - 1);

				pcurrent_frame->sp ++;
				pcurrent_frame->pc ++;
				break;
			}
		case   DUP_X1:            //0x5A        
			{
				goto unsupport;
			}
		case   DUP_X2:            //0x5B        
			{
				goto unsupport;
			}
		case   DUP2:              //0x5C      
			{
				goto unsupport;
			}
		case   DUP2_X1:           //0x5D         
			{
				goto unsupport;
			}
		case   DUP2_X2:           //0x5E         
			{
				goto unsupport;
			}
		case   SWAP:              //0x5F      

			//math:
		case   IADD:              //0x60      
			{
				int value1 = *(--pcurrent_frame->sp);
				int value2 = *(--pcurrent_frame->sp);

				*(pcurrent_frame->sp ++) = value1 + value2;
				pcurrent_frame->pc ++;

				break;
			}
		case   LADD:              //0x61      
			{
				goto unsupport;
			}
		case   FADD:              //0x62      
			{
				goto unsupport;
			}
		case   DADD:              //0x63      
			{
				goto unsupport;
			}
		case   ISUB:              //0x64      
			{
				goto unsupport;
			}
		case   LSUB:              //0x65      
			{
				goto unsupport;
			}
		case   FSUB:              //0x66      
			{
				goto unsupport;
			}
		case   DSUB:              //0x67      
			{
				goto unsupport;
			}
		case   IMUL:              //0x68      
			{
				goto unsupport;
			}
		case   LMUL:              //0x69      
			{
				goto unsupport;
			}
		case   FMUL:              //0x6A      
			{
				goto unsupport;
			}
		case   DMUL:              //0x6B      
			{
				goto unsupport;
			}
		case   IDIV:              //0x6C      
			{
				goto unsupport;
			}
		case   LDIV:              //0x6D      
			{
				goto unsupport;
			}
		case   FDIV:              //0x6E      
			{
				goto unsupport;
			}
		case   DDIV:              //0x6F      
			{
				goto unsupport;
			}
		case   IREM:              //0x70      
			{
				goto unsupport;
			}
		case   LREM:              //0x71      
			{
				goto unsupport;
			}
		case   FREM:              //0x72      
			{
				goto unsupport;
			}
		case   DREM:              //0x73      
			{
				goto unsupport;
			}
		case   INEG:              //0x74      
			{
				goto unsupport;
			}
		case   LNEG:              //0x75      
			{
				goto unsupport;
			}
		case   FNEG:              //0x76      
			{
				goto unsupport;
			}
		case   DNEG:              //0x77      
			{
				goto unsupport;
			}
		case   ISHL:              //0x78      
			{
				goto unsupport;
			}
		case   LSHL:              //0x79      
			{
				goto unsupport;
			}
		case   ISHR:              //0x7A      
			{
				goto unsupport;
			}
		case   LSHR:              //0x7B      
			{
				goto unsupport;
			}
		case   IUSHR:             //0x7C       
			{
				goto unsupport;
			}
		case   LUSHR:             //0x7D       
			{
				goto unsupport;
			}
		case   IAND:              //0x7E      
			{
				goto unsupport;
			}
		case   LAND:              //0x7F      
			{
				goto unsupport;
			}
		case   IOR:               //0x80     
			{
				goto unsupport;
			}
		case   LOR:               //0x81     
			{
				goto unsupport;
			}
		case   IXOR:              //0x82      
			{
				goto unsupport;
			}
		case   LXOR:              //0x83      
			{
				goto unsupport;
			}
		case   IINC:              //0x84      

			//conversation:
		case   I2L:               //0x85     
			{
				goto unsupport;
			}
		case   I2F:               //0x86     
			{
				goto unsupport;
			}
		case   I2D:               //0x87     
			{
				goto unsupport;
			}
		case   L2I:               //0x88     
			{
				goto unsupport;
			}
		case   L2F:               //0x89     
			{
				goto unsupport;
			}
		case   L2D:               //0x8A     
			{
				goto unsupport;
			}
		case   F2I:               //0x8B     
			{
				goto unsupport;
			}
		case   F2L:               //0x8C     
			{
				goto unsupport;
			}
		case   F2D:               //0x8D     
			{
				goto unsupport;
			}
		case   D2I:               //0x8E     
			{
				goto unsupport;
			}
		case   D2L:               //0x8F     
			{
				goto unsupport;
			}
		case   D2F:               //0x90     
			{
				goto unsupport;
			}
		case   I2B:               //0x91     
			{
				goto unsupport;
			}
		case   I2C:               //0x92     
			{
				goto unsupport;
			}
		case   I2S:               //0x93     

			//comparision:
		case   LCMP:              //0x94      
			{
				goto unsupport;
			}
		case   FCMPL:             //0x95       
			{
				goto unsupport;
			}
		case   FCMPG:             //0x96       
			{
				goto unsupport;
			}
		case   DCMPL:             //0x97       
			{
				goto unsupport;
			}
		case   DCMPG:             //0x98       
			{
				goto unsupport;
			}
		case   IFEQ:              //0x99      
			{
				goto unsupport;
			}
		case   IFNE:              //0x9A      
			{
				goto unsupport;
			}
		case   IFLT:              //0x9B      
			{
				goto unsupport;
			}
		case   IFGE:              //0x9C      
			{
				goto unsupport;
			}
		case   IFGT:              //0x9D      
			{
				goto unsupport;
			}
		case   IFLE:              //0x9E      
			{
				goto unsupport;
			}
		case   IF_ICMPEQ:         //0x9F           
			{
				goto unsupport;
			}
		case   IF_ICMPNE:         //0xA0           
			{
				goto unsupport;
			}
		case   IF_ICMPLT:         //0xA1           
			{
				goto unsupport;
			}
		case   IF_ICMPGE:         //0xA2           
			{
				goto unsupport;
			}
		case   IF_ICMPGT:         //0xA3           
			{
				goto unsupport;
			}
		case   IF_ICMPLE:         //0xA4           
			{
				goto unsupport;
			}
		case   IF_ACMPEQ:         //0xA5           
			{
				goto unsupport;
			}
		case   IF_ACMPNE:         //0xA6           

			//control:
		case   GOTO:              //0xA7      
			{
				goto unsupport;
			}
		case   JSR:               //0xA8     
			{
				goto unsupport;
			}
		case   RET:               //0xA9     
			{
				goto unsupport;
			}
		case   TABLESWITCH:       //0xAA             
			{
				goto unsupport;
			}
		case   LOOKUPSWITCH:      //0xAB              
			{
				goto unsupport;
			}
		case   IRETURN:           //0xAC         
			{
				// TODO check whether current method is clinit or not
				// and set class's status

				*(pcurrent_frame->fp->sp) = *(--pcurrent_frame->sp);
				printf("ireturn: %d\n", *(pcurrent_frame->fp->sp));
				pcurrent_frame->fp->sp ++;
				pcurrent_frame->pc ++;

				pop_frame(pcurrent_stack, update_current_frame);
				//update_current_frame();


				break;
			}
		case   LRETURN:           //0xAD         
			{
				goto unsupport;
			}
		case   FRETURN:           //0xAE         
			{
				goto unsupport;
			}
		case   DRETURN:           //0xAF         
			{
				goto unsupport;
			}
		case   ARETURN:           //0xB0         
			{
				goto unsupport;
			}
		case   RETURN:            //0xB1        
			{

				pcurrent_frame->pc ++;
				if (pop_frame(pcurrent_stack, update_current_frame))
				{
					break;
				}
				else
				{
					return;

				}
			}

			//reference:
		case   GETSTATIC:         //0xB2           
			{
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_field_info = (struct constant_fieldref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				// pclass have been resolved when the class is linked
				if (pconstant_field_info->pclass->status < CLASS_PREPARED)
				{
					prepare(pconstant_field_info->pclass);
				}

				if (pconstant_field_info->offset == 0xFFFF)
				{
					resolution_field(pconstant_field_info);
				}

				if (pconstant_field_info->pclass->status < CLASS_INITIALIZING)
				{
					// attention: need to re-interpret the GETSTATIC instruction
					pcurrent_frame->pc = pcurrent_frame->pc - 2;
					initialize(pconstant_field_info->pclass, pcurrent_stack);
				}
				else
				{
					//TODO now only support int(for test), need more support
					*pcurrent_frame->sp = *(u4* )((u1* )pconstant_field_info->pclass->pclass_instance->pvalues + pconstant_field_info->offset);
					pcurrent_frame->sp ++;
					pcurrent_frame->pc ++;
				}


				break;
			}
		case   PUTSTATIC:         //0xB3           
			{
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_field_info = (struct constant_fieldref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				// pclass have been resolved when the class is linked
				if (pconstant_field_info->pclass->status < CLASS_PREPARED)
				{
					prepare(pconstant_field_info->pclass);
				}

				if (pconstant_field_info->offset == 0xFFFF)
				{
					resolution_field(pconstant_field_info);
				}

				if (pconstant_field_info->pclass->status < CLASS_INITIALIZING)
				{
					// attention: need to re-interpret the PUTSTATIC instruction
					pcurrent_frame->pc = pcurrent_frame->pc - 2;
					initialize(pconstant_field_info->pclass, pcurrent_stack);
				}
				else
				{
					//TODO now only support int(for test), need more support
					*(u4* )((u1* )pconstant_field_info->pclass->pclass_instance->pvalues + pconstant_field_info->offset) = *(--pcurrent_frame->sp);
					pcurrent_frame->pc ++;
				}


				break;
			}
		case   GETFIELD:          //0xB4          
			{
				//TODO need more check, array?
				Object* pobject = *(Object** )(-- pcurrent_frame->sp);
				index = (((*(pcurrent_frame->pc + 1)) << 8) & 0xFF00) | (*(pcurrent_frame->pc + 2) & 0x00FF);
				pconstant_field_info = (struct constant_fieldref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				if (pconstant_field_info->offset == 0xFFFF)
				{
					resolution_field(pconstant_field_info);
				}

				//TODO now only support int
				*(int* )(pcurrent_frame->sp ++) = *(int* )((u1* )pobject->pvalues + pconstant_field_info->offset);

				pcurrent_frame->pc += 3;

				break;
			}
		case   PUTFIELD:          //0xB5          
			{
				Object* pobject;
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_field_info = (struct constant_fieldref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				// TODO need more check
				// TODO now only support int, specially the long and double(the size)

				if (pconstant_field_info->offset == 0xFFFF)
				{
					resolution_field(pconstant_field_info);
				}

				pobject = *(Object** )(pcurrent_frame->sp - 2);
				*(int* )((u1* )pobject->pvalues + pconstant_field_info->offset) = *(int* )(pcurrent_frame->sp - 1);

				pcurrent_frame->sp -= 2;
				pcurrent_frame->pc ++;

				break;
			}
		case   INVOKEVIRTUAL:     //0xB6               
			{
				//TODO need more check
				u2 parameters_size;
				struct Class* ptmp_class;
				Object* pobject;
				struct method_info* pmethod_info = NULL;
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_methodref_info = (struct constant_methodref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				// the class has not been prepared
				if (pclass->status < CLASS_PREPARED)
				{
					prepare(pclass);
				}

				// the constant pool entry has not been resolved
				if (pconstant_methodref_info->pmethod_info == NULL)
				{
					resolution_method(pclass, pconstant_methodref_info);
				}

				parameters_size = get_parameters_size(pconstant_methodref_info->pmethod_info);
				pobject = *(Object** )(pcurrent_frame->sp - 3);
				ptmp_class = pobject->pclass;

				while(ptmp_class)
				{
					pmethod_info = find_class_method(ptmp_class, pconstant_methodref_info->pconstant_name_and_type_info->pname_constant_utf8_info->pbytes,
						pconstant_methodref_info->pconstant_name_and_type_info->pname_constant_utf8_info->length,
						pconstant_methodref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info->pbytes,
						pconstant_methodref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info->length);
					if (pmethod_info)
					{
						break;
					}
					else
					{
						ptmp_class = ptmp_class->psuper_class;
					}

				}
				pcurrent_frame->pc ++;
				
				// TODO the current frame's sp will be modify in push_frame.
				// move it here
				push_frame(pcurrent_stack, ptmp_class, pmethod_info, update_current_frame);

				
				break;
			}
		case   INVOKESPECIAL:     //0xB7               
			{
				//TODO need more check, p483

				struct Class* psuper_class;
				struct method_info* pmethod_info = NULL;
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_methodref_info = (struct constant_methodref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				// the class has not been prepared
				if (pclass->status < CLASS_PREPARED)
				{
					prepare(pclass);
				}

				// the constant pool entry has not been resolved
				if (pconstant_methodref_info->pmethod_info == NULL)
				{
					resolution_method(pclass, pconstant_methodref_info);
				}

				//Let C be the direct superclass of the current class
				psuper_class = pcurrent_frame->pclass->psuper_class;

				while(psuper_class)
				{
					pmethod_info = find_class_method(psuper_class, pconstant_methodref_info->pconstant_name_and_type_info->pname_constant_utf8_info->pbytes,
						pconstant_methodref_info->pconstant_name_and_type_info->pname_constant_utf8_info->length,
						pconstant_methodref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info->pbytes,
						pconstant_methodref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info->length);
					if (pmethod_info)
					{
						break;
					}
					else
					{
						psuper_class = psuper_class->psuper_class;
					}

				}
				pcurrent_frame->pc ++;
				
				// TODO the current frame's sp will be modify in push_frame.
				// move it here
				push_frame(pcurrent_stack, psuper_class, pmethod_info, update_current_frame);

				break;
			}
		case   INVOKESTATIC:      //0xB8              
			{
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				pconstant_methodref_info = (struct constant_methodref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;
				
				pclass = pconstant_methodref_info->pclass;
				if (pclass == NULL)
				{
					// TODO can not find method
				}
				else
				{
					// the class has not been prepared
					if (pclass->status < CLASS_PREPARED)
					{
						prepare(pclass);
					}

					// the constant pool entry has not been resolved
					if (pconstant_methodref_info->pmethod_info == NULL)
					{
						resolution_method(pclass, pconstant_methodref_info);
					}
					
					// TODO do more check here: is native, abstract and so on?
					pcurrent_frame->pc ++;
					push_frame(pcurrent_stack, pclass, pconstant_methodref_info->pmethod_info, update_current_frame);
					//update_current_frame();

					//TODO initialize the class
					if (pclass->status < CLASS_INITIALIZING)
					{
						initialize(pclass, pcurrent_stack);
					}
					
				}

				break;
			}

		case   INVOKEINTERFACE:   //0xB9                 
			{
				goto unsupport;
			}
		case   INVOKEDYNAMIC:     //0xBA               
			{
				goto unsupport;
			}
		case   NEW:               //0xBB     
			{
				index = (((*(++pcurrent_frame->pc)) << 8) & 0xFF00) | (*(++pcurrent_frame->pc) & 0x00FF);
				// TODO can not use pclass directly
				pconstant_class_info = (struct constant_class_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;

				pclass = pconstant_class_info->pclass;
				if (pclass == NULL)
				{
					// TODO can not find method
				}
				else
				{
					// the class has not been prepared
					if (pclass->status < CLASS_PREPARED)
					{
						prepare(pclass);
					}

					//TODO initialize the class
					if (pclass->status < CLASS_INITIALIZING)
					{
						// attention: need to re-interpret the NEW instruction
						pcurrent_frame->pc = pcurrent_frame->pc - 2;
						initialize(pclass, pcurrent_stack);
					}
					else
					{
						//TODO check whether the resolved class is an interface or not?
						Object* pobject = create_object(pconstant_class_info->pclass);
						*(Object** )(pcurrent_frame->sp) = pobject;
						
						pcurrent_frame->sp ++;
						pcurrent_frame->pc ++;
					}
					
				}
				break;
			}
		case   NEWARRAY:          //0xBC          
			{
				goto unsupport;
			}
		case   ANEWARRAY:         //0xBD           
			{
				goto unsupport;
			}
		case   ARRAYLENGTH:       //0xBE             
			{
				goto unsupport;
			}
		case   ATHROW:            //0xBF        
			{
				goto unsupport;
			}
		case   CHECKCAST:         //0xC0           
			{
				goto unsupport;
			}
		case   INSTANCEOF:        //0xC1            
			{
				goto unsupport;
			}
		case   MONITORENTER:      //0xC2              
			{
				goto unsupport;
			}
		case   MONITOREXIT:       //0xC3             

			//extend:
		case   WIDE:              //0xC4      
			{
				goto unsupport;
			}
		case   MULTIANEWARRAY:    //0xC5                
			{
				goto unsupport;
			}
		case   IFNULL:            //0xC6        
			{
				goto unsupport;
			}
		case   IFNONNULL:         //0xC7           
			{
				goto unsupport;
			}
		case   GOTO_W:            //0xC8        
			{
				goto unsupport;
			}
		case   JSR_W:             //0xC9       

			//reserve:
		case   BREAKPOINT:        //0xCA            
			{
				goto unsupport;
			}
		case   IMPDEP1:           //0xFE         
			{
				goto unsupport;
			}
		case   IMPDEP2:           //0xFF         
			{
				goto unsupport;
			}

		default:
unsupport:
			printf("error: do not support instruction: %02x", ins);
			return;
		}

		ins = fetch();
		
	}
}
