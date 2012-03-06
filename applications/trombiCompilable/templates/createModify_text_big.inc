<?
//TODO Refaire la partie javascript
?>

<p class="form-textarea">
	<label for="id_/*id*/">/*text*/</label>
	<textarea name="/*id*/" id="id_/*id*/" 
			onBlur="Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);"
			onChange="Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);" 
			onFocus="Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);"
			onKeydown="Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);" 
			onKeyup="Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);"
			rows="/*rows*/"
			cols="/*cols*/"><?=str_replace(Array("<br />", "&nbsp;"), Array("\n", " "), getValue($values,"/*id*/"));?></textarea>
	<span>/*extraText*/</span>
	<label for="id_/*id*/"><?=getError($error,"/*id*/");?></label>
</p>
<p>
	<input type="text" size="3" name="aff_/*id*/" id="aff_/*id*/" value="" readonly="readonly" />
   <script  type="text/javascript">
  function Calcul_/*id*/(id_affich,id_compte,max_carac)
  {
    max_carac = max_carac || 550
    var iLongueurRestante,
	  input_comptage = document.getElementById(id_compte),
    iLongueur = input_comptage.value.length;
    if(iLongueur>max_carac)
    {
      input_comptage.value = input_comptage.value.substring(0,max_carac);
      iLongueurRestante = 0;
    }
    else
      iLongueurRestante = max_carac - iLongueur;
    document.getElementById(id_affich).value = iLongueurRestante;
  }
  Calcul_/*id*/('aff_/*id*/','id_/*id*/',/*maxSize*/);
	</script>
</p>
