#help_index "Info;Task"

I64 TaskMemCnt(CTask *task=NULL) {
  if(!task) task=Fs;
  if(!TaskValidate(task)) return 0;
  I64 retval=0;
  if(task->code_heap!=task->data_heap) {
    retval=task->code_heap->alloced_u8s+
	  task->data_heap->alloced_u8s;
  } else
    retval=task->code_heap->alloced_u8s;
  return retval;
} 
U0 TaskRepTask(CTask *task,I64 indent)
{
  CTask *task1;
  "%h*c$$MA,T=\"%08X\",LM=\"Kill(0x%X);\n\",$$ $$BLACK$$#%d$$FG$$ "
	"$$TX,\"%$$Q...\",SCX=16$$\n",indent,CH_SPACE,task,task,task->task_num,
	task->task_title;
  "%h*c%08X%04X:%04X:%08X\n",indent+2,CH_SPACE,TaskMemCnt(task), //Changed for EXODUS
	task->task_flags,task->display_flags,task->win_inhibit;
  task1=task->next_child_task;
  while (task1!=(&task->next_child_task)(U8 *)
	-offset(CTask.next_sibling_task)) {
    TaskRepTask(task1,indent+2);
    task1=task1->next_sibling_task;
  }
}

public U0 TaskRep()
{//Report current tasks on all cores.
  I64 i,bl;
  CCPU *c;
  bl=BreakLock;
  for (i=0;i<mp_cnt;i++) {
    c=&cpu_structs[i];
    "$$PURPLE$$CPU%02X$$FG$$\n",i;
    TaskRepTask(c->seth_task,2);
  }
  if(bl) BreakUnlock;
}
