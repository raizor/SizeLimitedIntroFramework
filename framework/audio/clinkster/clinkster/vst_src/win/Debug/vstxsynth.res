        ��  ��                    ,   V S T X M L   ��       0 	        <!-- =========================================================== -->
<!-- VST Parameter XML Definition for vstxsynth ================ -->
<!-- Version 1.0 =============================================== -->
<!-- Date: 01/2006 ============================================= -->
<!-- =========================================================== -->

<VSTPluginProperties>
	<VSTParametersStructure>
		<!-- ======================================================= -->
		<!-- Value Types =========================================== --> 
		<!-- ======================================================= -->

		<ValueType name="Waveform">
			<Entry name="Sawtooth" value="[0, 0.5["/> 
			<Entry name="Pulse" value="[0.5, 1]"/> 
		</ValueType>

		<!-- ======================================================= -->
		<!-- Templates ============================================= --> 
		<!-- ======================================================= -->

		<Template name="Osc">
			<Param name="Waveform" type="Waveform" id="offset"/>
			<Param name="Frequency" id="offset+1"/>
			<Param name="Level" id="offset+2"/>
		</Template>
		
		<!-- ======================================================= -->
		<!-- Global ================================================ -->
		<!-- ======================================================= -->

		<Param name="Volume" id="6"/>
		<Group name="Osc Left"  template="Osc" values="offset=0"/>
		<Group name="Osc Right" template="Osc" values="offset=3"/>
	</VSTParametersStructure>
</VSTPluginProperties>
