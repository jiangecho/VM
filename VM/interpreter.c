#include "interpreter.h"
#include "opcode.h"
#include "stack.h"
#include "attribute.h"
#include "class.h"
#include "method.h"

#include <stdio.h>

int pc = 0;
static struct frame* pcurrent_frame;
static struct stack* pcurrent_stack;
static struct method_info* pcurrent_method_info;
static struct code_attribute_info* pcode_attribute_info;
static struct Class* pcurrent_class;
u1* pcode;



void set_current_stack(struct stack* pstack)
{
	pcurrent_stack = pstack;
	pcurrent_frame = pstack->pcurrent_frame;
	pcurrent_method_info = pcurrent_frame->pmethod_info;

	pcode_attribute_info = get_code_attribute_info(pcurrent_method_info);
	pcode = pcode_attribute_info->pcode;
}

u1 fetch()
{
	u1 instruction = *(pcode + pc);
	return instruction;
}

void interpreter()
{
	struct constant_methodref_info* pconstant_methodref_info;
	struct constant_name_and_type_info* pconstant_name_and_type_info;
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
		case   ACONST_NULL:       //0x01             
		case   ICONST_M1:         //0x02           
		case   ICONST_0:          //0x03          
		case   ICONST_1:          //0x04          
		case   ICONST_2:          //0x05          
		case   ICONST_3:          //0x06          
			*((u1* )(pcurrent_frame + sizeof(struct frame) + pcurrent_frame->sp ++)) = 0xFF000000 & 3;
			*((u1* )(pcurrent_frame + sizeof(struct frame) + pcurrent_frame->sp ++)) = 0x00FF0000 & 3;
			*((u1* )(pcurrent_frame + sizeof(struct frame) + pcurrent_frame->sp ++)) = 0x0000FF00 & 3;
			*((u1* )(pcurrent_frame + sizeof(struct frame) + pcurrent_frame->sp ++)) = 0x000000FF & 3;
			pc ++;
			pcurrent_frame->pc = pc;


			break;

		case   ICONST_4:          //0x07          
		case   ICONST_5:          //0x08          
		case   LCONST_0:          //0x09          
		case   LCONST_1:          //0x0A          
		case   FCONST_0:          //0x0B          
		case   FCONST_1:          //0x0C          
		case   FCONST_2:          //0x0D          
		case   DCONST_0:          //0x0E          
		case   DCONST_1:          //0x0F          
		case   BIPUSH:            //0x10        
			*((u1* )(pcurrent_frame + sizeof(struct frame) + pcurrent_frame->sp ++)) = *(pcode + (++pc));
			pc ++;
			pcurrent_frame->pc = pc;

			break;

		case   SIPUSH:            //0x11        
		case   LDC:               //0x12     
		case   LDC_W:             //0x13       
		case   LDC2_W:            //0x14        

			//loads:
		case   ILOAD:             //0x15       
		case   LLOAD:             //0x16       
		case   FLOAD:             //0x17       
		case   DLOAD:             //0x18       
		case   ALOAD:             //0x19       
		case   ILOAD_0:           //0x1A         
		case   ILOAD_1:           //0x1B         
		case   ILOAD_2:           //0x1C         
		case   ILOAD_3:           //0x1D         
		case   LLOAD_0:           //0x1E         
		case   LLOAD_1:           //0x1F         
		case   LLOAD_2:           //0x20         
		case   LLOAD_3:           //0x21         
		case   FLOAD_0:           //0x22         
		case   FLOAD_1:           //0x23         
		case   FLOAD_2:           //0x24         
		case   FLOAD_3:           //0x25         
		case   DLOAD_0:           //0x26         
		case   DLOAD_1:           //0x27         
		case   DLOAD_2:           //0x28         
		case   DLOAD_3:           //0x29         
		case   ALOAD_0:           //0x2A         
		case   ALOAD_1:           //0x2B         
		case   ALOAD_2:           //0x2C         
		case   ALOAD_3:           //0x2D         
		case   IALOAD:            //0x2E        
		case   LALOAD:            //0x2F        
		case   FALOAD:            //0x30        
		case   DALOAD:            //0x31        
		case   AALOAD:            //0x32        
		case   BALOAD:            //0x33        
		case   CALOAD:            //0x34        
		case   SALOAD:            //0x35        

			//stores:
		case   ISTORE:            //0x36        
		case   LSTORE:            //0x37        
		case   FSTORE:            //0x38        
		case   DSTORE:            //0x39        
		case   ASTORE:            //0x3A        
		case   ISTORE_0:          //0x3B          
		case   ISTORE_1:          //0x3C          
		case   ISTORE_2:          //0x3D          
		case   ISTORE_3:          //0x3E          
		case   LSTORE_0:          //0x3F          
		case   LSTORE_1:          //0x40          
		case   LSTORE_2:          //0x41          
		case   LSTORE_3:          //0x42          
		case   FSTORE_0:          //0x43          
		case   FSTORE_1:          //0x44          
		case   FSTORE_2:          //0x45          
		case   FSTORE_3:          //0x46          
		case   DSTORE_0:          //0x47          
		case   DSTORE_1:          //0x48          
		case   DSTORE_2:          //0x49          
		case   DSTORE_3:          //0x4A          
		case   ASTORE_0:          //0x4B          
		case   ASTORE_1:          //0x4C          
		case   ASTORE_2:          //0x4D          
		case   ASTORE_3:          //0x4E          
		case   IASTORE:           //0x4F         
		case   LASTORE:           //0x50         
		case   FASTORE:           //0x51         
		case   DASTORE:           //0x52         
		case   AASTORE:           //0x53         
		case   BASTORE:           //0x54         
		case   CASTORE:           //0x55         
		case   SASTORE:           //0x56         

			//stack:
		case   POP:               //0x57     
		case   POP2:              //0x58      
		case   DUP:               //0x59     
		case   DUP_X1:            //0x5A        
		case   DUP_X2:            //0x5B        
		case   DUP2:              //0x5C      
		case   DUP2_X1:           //0x5D         
		case   DUP2_X2:           //0x5E         
		case   SWAP:              //0x5F      

			//math:
		case   IADD:              //0x60      
		case   LADD:              //0x61      
		case   FADD:              //0x62      
		case   DADD:              //0x63      
		case   ISUB:              //0x64      
		case   LSUB:              //0x65      
		case   FSUB:              //0x66      
		case   DSUB:              //0x67      
		case   IMUL:              //0x68      
		case   LMUL:              //0x69      
		case   FMUL:              //0x6A      
		case   DMUL:              //0x6B      
		case   IDIV:              //0x6C      
		case   LDIV:              //0x6D      
		case   FDIV:              //0x6E      
		case   DDIV:              //0x6F      
		case   IREM:              //0x70      
		case   LREM:              //0x71      
		case   FREM:              //0x72      
		case   DREM:              //0x73      
		case   INEG:              //0x74      
		case   LNEG:              //0x75      
		case   FNEG:              //0x76      
		case   DNEG:              //0x77      
		case   ISHL:              //0x78      
		case   LSHL:              //0x79      
		case   ISHR:              //0x7A      
		case   LSHR:              //0x7B      
		case   IUSHR:             //0x7C       
		case   LUSHR:             //0x7D       
		case   IAND:              //0x7E      
		case   LAND:              //0x7F      
		case   IOR:               //0x80     
		case   LOR:               //0x81     
		case   IXOR:              //0x82      
		case   LXOR:              //0x83      
		case   IINC:              //0x84      

			//conversation:
		case   I2L:               //0x85     
		case   I2F:               //0x86     
		case   I2D:               //0x87     
		case   L2I:               //0x88     
		case   L2F:               //0x89     
		case   L2D:               //0x8A     
		case   F2I:               //0x8B     
		case   F2L:               //0x8C     
		case   F2D:               //0x8D     
		case   D2I:               //0x8E     
		case   D2L:               //0x8F     
		case   D2F:               //0x90     
		case   I2B:               //0x91     
		case   I2C:               //0x92     
		case   I2S:               //0x93     

			//comparision:
		case   LCMP:              //0x94      
		case   FCMPL:             //0x95       
		case   FCMPG:             //0x96       
		case   DCMPL:             //0x97       
		case   DCMPG:             //0x98       
		case   IFEQ:              //0x99      
		case   IFNE:              //0x9A      
		case   IFLT:              //0x9B      
		case   IFGE:              //0x9C      
		case   IFGT:              //0x9D      
		case   IFLE:              //0x9E      
		case   IF_ICMPEQ:         //0x9F           
		case   IF_ICMPNE:         //0xA0           
		case   IF_ICMPLT:         //0xA1           
		case   IF_ICMPGE:         //0xA2           
		case   IF_ICMPGT:         //0xA3           
		case   IF_ICMPLE:         //0xA4           
		case   IF_ACMPEQ:         //0xA5           
		case   IF_ACMPNE:         //0xA6           

			//control:
		case   GOTO:              //0xA7      
		case   JSR:               //0xA8     
		case   RET:               //0xA9     
		case   TABLESWITCH:       //0xAA             
		case   LOOKUPSWITCH:      //0xAB              
		case   IRETURN:           //0xAC         
		case   LRETURN:           //0xAD         
		case   FRETURN:           //0xAE         
		case   DRETURN:           //0xAF         
		case   ARETURN:           //0xB0         
		case   RETURN:            //0xB1        

			//reference:
		case   GETSTATIC:         //0xB2           
		case   PUTSTATIC:         //0xB3           
		case   GETFIELD:          //0xB4          
		case   PUTFIELD:          //0xB5          
		case   INVOKEVIRTUAL:     //0xB6               
		case   INVOKESPECIAL:     //0xB7               
		case   INVOKESTATIC:      //0xB8              

			//TODO
			index = (((*(pcode + (++pc))) << 8) & 0xFF00) | (*(pcode + (++pc)) & 0x00FF);
			pc ++;
			pcurrent_frame->pc = pc;

			pconstant_methodref_info = (struct constant_methodref_info* )pcurrent_frame->pclass->pcp_info[index].pinfo;
			
			break;

		case   INVOKEINTERFACE:   //0xB9                 
		case   INVOKEDYNAMIC:     //0xBA               
		case   NEW:               //0xBB     
		case   NEWARRAY:          //0xBC          
		case   ANEWARRAY:         //0xBD           
		case   ARRAYLENGTH:       //0xBE             
		case   ATHROW:            //0xBF        
		case   CHECKCAST:         //0xC0           
		case   INSTANCEOF:        //0xC1            
		case   MONITORENTER:      //0xC2              
		case   MONITOREXIT:       //0xC3             

			//extend:
		case   WIDE:              //0xC4      
		case   MULTIANEWARRAY:    //0xC5                
		case   IFNULL:            //0xC6        
		case   IFNONNULL:         //0xC7           
		case   GOTO_W:            //0xC8        
		case   JSR_W:             //0xC9       

			//reserve:
		case   BREAKPOINT:        //0xCA            
		case   IMPDEP1:           //0xFE         
		case   IMPDEP2:           //0xFF         

		default:
			printf("do not support instruction: %02x", ins);
			break;
		}

		ins = fetch();
		
	}
}